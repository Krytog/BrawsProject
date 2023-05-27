#include "Porter.h"
#include <boost/asio/write.hpp>
#include <iostream>
#include <mutex>

using boost::asio::ip::tcp;

Porter::Lobby::Lobby(uint64_t users_count) : users_count_(users_count) {
}

Porter::Lobby::Lobby(const GameSettings& settings) : Lobby(settings.users_count) {
    /* Handle other settings somehow */
}

void Porter::Lobby::SetPlayerCount(uint64_t users_count) {
    users_count_ = users_count;
}

void Porter::Lobby::AddPlayer(Player player) {
    if (players_.contains(player.id)) {
        std::cout << "Cannot register player twice" << std::endl;
    } else {
        players_[player.id] = std::move(player);
    }
}

bool Porter::Lobby::Ready() const {
    return players_.size() == users_count_;
}

bool Porter::Lobby::RemovePlayer(uint64_t id) {
    return players_.erase(id);
}

void Porter::Lobby::Clear() {
    players_.clear();
}

Porter::Lobby::Status Porter::Lobby::GetStatus() const {
    return status_;
}

void Porter::Lobby::SetStatus(Porter::Lobby::Status status) {
    status_ = status;
}

std::unordered_map<uint64_t, Player> Porter::Lobby::GetPlayers() const {
    return players_;
}

Porter& Porter::GetInstance() {
    static Porter instance;
    return instance;
}

Porter::Porter() : acceptor_(io_context_, tcp::endpoint(tcp::v4(), GAME_PORT)), rd_(), gen_(rd_()), dis_() {
    ipc::shared_memory_object::remove("SharedMemorySegment");
    shared_segment_ = shared::Segment(ipc::create_only, "SharedMemorySegment", kLobbyHashMapMaxSize);
    shared::Manager* segment_manager = shared_segment_.get_segment_manager();
    lobby_status_ = shared_segment_.construct<SharedLobbyStatusHashMap>("SharedHashMap")(segment_manager);
}

void Porter::RegUser() {
    tcp::socket connection(io_context_);
    uint64_t user_id = RegId();
    uint16_t user_port;
    acceptor_.accept(connection);

    boost::asio::write(connection, boost::asio::buffer(&user_id, sizeof(user_id)));
    boost::asio::read(connection, boost::asio::buffer(&user_port, sizeof(user_port)));
    ports_[user_id] = user_port;

    connection.non_blocking(true);

    std::scoped_lock guard(reg_mutex_);
    incoming_users_.emplace(user_id, std::move(connection));
    has_incoming_users_.store(true);
}

void Porter::HandleConnection(uint64_t user_id, tcp::socket& connection, const Request& header) {
    udp::endpoint endpoint(connection.local_endpoint().address(), ports_[user_id]);

    if (header.type == RequestType::ConnectToGame) {
        std::scoped_lock guard(lobby_lock_);
        if (!lobbies_.contains(header.id)) {  // Invalid lobby ID
            std::cout << "Invalid Game ID" << std::endl;
            return;
        }
        if (players_[user_id] != kGameUndefined) {  // user is in another lobby
            lobbies_.at(players_[user_id]).RemovePlayer(user_id);
            players_[user_id] = kGameUndefined;
        }
        if (header.id == kGameUndefined) {  // join any game
            for (auto& [lobby_id, lobby] : lobbies_) {
                if (lobby.GetStatus() == Lobby::Waiting) {
                    lobbies_.at(lobby_id).AddPlayer(
                        {.id = user_id, .endpoint = endpoint, .character = header.character_type});
                    players_[user_id] = lobby_id;
                }
            }
        } else {  // join game with ID == header.id
            if (lobbies_.at(header.id).Ready()) return;  // cannot connect to Ready lobby
            lobbies_.at(header.id).AddPlayer(
                {.id = user_id, .endpoint = endpoint, .character = header.character_type});
            players_[user_id] = header.id;
        }
    } else if (header.type == RequestType::CreateNewGame) {
        if (players_[user_id] != kGameUndefined) {  // user is in another lobby
            std::scoped_lock guard(lobby_lock_);
            lobbies_.at(players_[user_id]).RemovePlayer(user_id);
        }
        uint64_t lobby_id = RegLobbyId();
        std::cout << "Your lobby ID is " << lobby_id << std::endl;

        GameSettings settings;
        connection.non_blocking(false);  // block until user sends all data
        boost::asio::read(connection, boost::asio::buffer(&settings, sizeof(settings)));
        boost::asio::write(connection, boost::asio::buffer(&lobby_id, sizeof(lobby_id)));
        connection.non_blocking(true);  // unblock for further request handling

        std::scoped_lock guard(lobby_lock_);
        lobbies_.emplace(lobby_id, settings);
        lobbies_.at(lobby_id).AddPlayer(
            {.id = user_id, .endpoint = endpoint, .character = header.character_type});
        players_[user_id] = lobby_id;
    } else if (header.type == RequestType::LeaveGame) {
        /* Add Checks */
        std::scoped_lock guard(lobby_lock_);
        lobbies_.at(players_[user_id]).RemovePlayer(user_id);
        players_.erase(user_id);  // need faster
    } else {
        /* Handle invalid request - do nothing */
    }
}

void Porter::HandleRequests() {
    if (has_incoming_users_) {
        std::scoped_lock guard(reg_mutex_);
        for (auto& [user_id, connection] : incoming_users_) {
            connections_.emplace(user_id, std::move(connection));
        }
        incoming_users_.clear();
        has_incoming_users_.store(false);
    }
    std::cout << connections_.size() << std::endl;
    std::erase_if(connections_, [this](std::pair<const uint64_t, tcp::socket>& p) {
        Request header;
        const uint64_t& user_id = p.first;
        tcp::socket& connection = p.second;

        boost::system::error_code ec;
        boost::asio::read(connection, boost::asio::buffer(&header, sizeof(header)), ec);  // handle exceptions using error codes
        if (ec == boost::asio::error::would_block) { return false; } // empty socket, but still connected

        if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset ||
            header.type == RequestType::EndGameSession) {  // tcp socket disconnection or final package arrival
            std::scoped_lock guard(lobby_lock_);
            if (players_[user_id] != kGameUndefined) {                  // user is still in lobby
                lobbies_.at(players_[user_id]).RemovePlayer(user_id);
            }
            players_.erase(user_id);
            ports_.erase(user_id);
            return true;
        }
        HandleConnection(user_id, connection, header);
        return false;
    });
}

void Porter::StartRegistration() {
    reg_thread_ = std::thread([this]() {
        while (true) {
            RegUser();
        }
    });
}

void Porter::StartHandling() {
    accept_thread_ = std::thread([this] {
        while (true) {
            HandleRequests();
        }
    });
}

uint64_t Porter::RegId() {
    do {
        uint64_t num = dis_(gen_);
        if (!ports_.contains(num)) {
            return num;
        }
    } while (true);
}

uint64_t Porter::RegLobbyId() { /* избавиться от копипасты потом */
    do {
        uint64_t num = dis_(gen_);
        if (num == kGameUndefined || lobbies_.contains(num)) {
            continue;
        }
        return num;
    } while (true);
}

void Porter::CheckLobbiesState() {
    std::scoped_lock guard(lobby_lock_);
    std::erase_if(lobbies_, [this](std::pair<const uint64_t, Lobby>& p) {
        Lobby& lobby = p.second;
        if ((*lobby_status_)[p.first].load()) {
            lobby_status_->erase(p.first);
            lobby.SetStatus(Lobby::Finished);
        }
        if (lobby.GetStatus() == Lobby::Finished) {
            for (const auto& [id, player]: lobby.GetPlayers()) {
                players_[id] = kGameUndefined;  // the game ended, but user can start a new one
            }
            return true;
        }
        if (lobby.Ready() && lobby.GetStatus() != Lobby::Running) {
            lobby.SetStatus(Lobby::Running);
            lobby_status_->emplace(p.first, false);
            InitGame(p.first, lobby);
        }
        return false;
    });
}

void Porter::InitGame(uint64_t lobby_id, Porter::Lobby& lobby) {
    SendInitGamePackages(lobby);
    if (!fork()) {
        int nullfd = open("/dev/null", O_WRONLY);
        dup2(nullfd, STDERR_FILENO);  // ignore errors
        close(nullfd);

        std::cout << "Game Started" << std::endl;
        Game(lobby.GetPlayers());

        shared::Segment segment(ipc::open_only, "SharedMemorySegment");
        SharedLobbyStatusHashMap* hashmap = segment.find<SharedLobbyStatusHashMap>("SharedHashMap").first;
        (*hashmap)[lobby_id].store(true);
        std::cout << "Game Ended" << std::endl;

        /* Collect statistics */

        exit(0);  // here std::system_error throws because of threads, we ignore it
    }
}

void Porter::SendInitGamePackages(const Lobby& lobby) {
    const auto& players = lobby.GetPlayers();
    for (const auto& [id, player] : players) {
        boost::asio::write(connections_.at(id), boost::asio::buffer(GAME_APPROVE));
    }
}

Porter::~Porter() {
    reg_thread_.join();
    accept_thread_.join();

    shared_segment_.destroy<SharedLobbyStatusHashMap>("SharedHashMap");
    ipc::shared_memory_object::remove("SharedMemorySegment");
}