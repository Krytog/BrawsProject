#include "Porter.h"
#include <boost/asio/write.hpp>
#include <iostream>
#include <mutex>

using boost::asio::ip::tcp;

Porter::Lobby::Lobby(size_t users_count) : users_count_(users_count) {
}

void Porter::Lobby::SetPlayerCount(size_t users_count) {
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
}

void Porter::RegUser() {
    tcp::socket connection(io_context_);
    uint64_t user_id = RegId();
    acceptor_.accept(connection);
    boost::asio::write(connection, boost::asio::buffer(&user_id, sizeof(user_id)));
    std::cout << user_id << std::endl;

    connection.non_blocking(true);

    std::scoped_lock guard(reg_mutex_);
    incoming_users_.emplace(user_id, std::move(connection));
    has_incoming_users_.store(true);
}

void Porter::HandleRequest() {
    if (has_incoming_users_) {
        std::scoped_lock guard(reg_mutex_);
        for (auto& [user_id, connection] : incoming_users_) {
            connections_.emplace(user_id, std::move(connection));
        }
        incoming_users_.clear();
        has_incoming_users_.store(false);
    }

    for (auto& [user_id, connection] : connections_) {
        Request header;

        boost::system::error_code er;
        boost::asio::read(connection, boost::asio::buffer(&header, sizeof(header)), er);
        if (er == boost::asio::error::would_block || er == boost::asio::error::eof) {
            continue;
        }

        udp::endpoint endpoint(connection.local_endpoint().address(), connection.local_endpoint().port());

        if (header.type == RequestType::ConnectToGame) {
            std::scoped_lock guard(wait_requests_);
            if (players_[user_id] != 0) {  // user is in another lobby
                lobbies_.at(players_[user_id]).RemovePlayer(user_id);
                players_.erase(user_id);
            }
            if (header.id == 0) {  // join any game
                for (auto& [lobby_id, lobby]: lobbies_) {
                    if (lobby.GetStatus() == Lobby::Waiting) {
                        lobbies_.at(lobby_id).AddPlayer({.id = user_id, .endpoint = endpoint, .character = header.character_type});
                        players_[user_id] = lobby_id;
                    }
                }
            } else {
                if (lobbies_.at(header.id).Ready()) continue;  // cannot connect to Ready lobby
                lobbies_.at(header.id).AddPlayer(
                    {.id = user_id, .endpoint = endpoint, .character = header.character_type});
                players_[user_id] = header.id;
            }
        } else if (header.type == RequestType::CreateNewGame) {
            if (players_[user_id] != 0) {  // user is in another lobby
                std::scoped_lock guard(wait_requests_);
                lobbies_.at(players_[user_id]).RemovePlayer(user_id);
            }
            uint64_t lobby_id = RegLobbyId();
            std::cout << lobby_id << std::endl;

            GameSettings settings;
            connection.non_blocking(false);  // block until user sends all data
            boost::asio::read(connection, boost::asio::buffer(&settings, sizeof(settings)));
            boost::asio::write(connection, boost::asio::buffer(&lobby_id, sizeof(lobby_id)));
            connection.non_blocking(true);  // unblock for further request handling

            std::scoped_lock guard(wait_requests_);
            lobbies_.emplace(lobby_id, settings.users_count);
            lobbies_.at(lobby_id).AddPlayer(
                {.id = user_id, .endpoint = endpoint, .character = header.character_type});
            players_[user_id] = lobby_id;
        } else if (header.type == RequestType::LeaveGame) {
            std::scoped_lock guard(wait_requests_);
            lobbies_.at(players_[user_id]).RemovePlayer(user_id);
            players_.erase(user_id);  // need faster
        } else {
            /* Handle invalid request - do nothing */
        }
    }
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
            HandleRequest();
        }
    });
}

uint64_t Porter::RegId() {
    do {
        uint64_t num = dis_(gen_);
        if (!used_user_ids_.contains(num)) {
            used_user_ids_.insert(num);
            return num;
        }
    } while (true);
}

uint64_t Porter::RegLobbyId() { /* избавиться от копипасты потом */
    do {
        uint64_t num = dis_(gen_);
        if (!num || lobbies_.contains(num)) {
            continue;
        }
        return num;
    } while (true);
}

void Porter::CheckLobbiesState() {
    std::scoped_lock guard(wait_requests_);
    for (auto& [lobby_id, lobby] : lobbies_) {
        if (lobby.Ready()) {
            lobby.SetStatus(Lobby::Running);
            InitGame(lobby);
        } else if (lobby.GetStatus() == Lobby::Finished) {
            lobby.Clear();
        }
    }
}

void Porter::InitGame(Porter::Lobby& lobby) {
    SendInitGamePackages(lobby);
    if (!fork()) {
        Game(lobby.GetPlayers());
        lobby.SetStatus(Lobby::Finished);
        /* Collect statistics */
    }
}

void Porter::SendInitGamePackages(const Lobby& lobby) {
    const auto& players = lobby.GetPlayers();
    std::string message = "start";
    for (const auto& [id, player]: players) {
        boost::asio::write(connections_[id], boost::asio::buffer(message.data(), message.size()));
    }
}


Porter::~Porter() {
    reg_thread_.join();
    accept_thread_.join();
}