#include "Porter.h"
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

namespace {
    uint64_t random_port = 10000;
}

Porter::Lobby::Lobby(size_t users_count) : users_count_(users_count) {}

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

Porter &Porter::GetInstance() {
    static Porter instance;
    return instance;
}

Porter::Porter(): acceptor_(io_context_, tcp::endpoint(tcp::v4(), random_port)),
    rd_(), gen_(rd_()), dis_() {}


void Porter::RegUser() {
    tcp::socket connection(io_context_);
    uint64_t user_id = RegId();
    acceptor_.accept(connection);

    boost::asio::write(connection, boost::asio::buffer(&user_id, sizeof(user_id)));

    Request header;
    boost::asio::read(connection, boost::asio::buffer(&header, sizeof(header)));
    udp::endpoint endpoint(connection.local_endpoint().address(),
        connection.local_endpoint().port());

    if (header.type == RequestType::ConnectToGame) {
        wait_reg_.lock();
        lobbies_.at(header.id).AddPlayer({
            .id = header.id,
            .endpoint = endpoint,
            .character = header.character_type   
        });
        has_incoming_users_.store(true);
        wait_reg_.unlock();
    } else if (header.type == RequestType::CreateNewGame) {
        wait_reg_.lock();
        uint64_t lobby_id = RegLobbyId();
        std::cout << lobby_id << std::endl;
        LobbySettings settings;
        boost::asio::read(connection, boost::asio::buffer(&settings, sizeof(settings)));
        lobbies_.emplace(lobby_id, settings.users_count);

        lobbies_.at(lobby_id).AddPlayer({
            .id = header.id,
            .endpoint = endpoint,
            .character = header.character_type   
        });

        boost::asio::write(connection, boost::asio::buffer(&lobby_id, sizeof(lobby_id)));
        has_incoming_users_.store(true);
        wait_reg_.unlock();
    } else {
        /* Кинуть наверное челу ошибку */
    }
}

void Porter::StartRegistration() {
    reg_thread_ = std::thread([this]() {
        while (true) {
            RegUser();
        }
    });
}

uint64_t Porter::RegId() {
    do {
        uint64_t num = dis_(gen_);
        if (!connections_.contains(num)) {
            return num;
        }
    } while (true);
}

uint64_t Porter::RegLobbyId() { /* избавиться от копипасты потом */
    do {
        uint64_t num = dis_(gen_);
        if (!lobbies_.contains(num)) {
            return num;
        }
    } while (true);
}

void Porter::CheckLobbiesState() {
    if (has_incoming_users_.load()) {
        wait_reg_.lock();
        for (auto& [lobby_id, lobby]: lobbies_) {
            if (lobby.Ready()) {
                /* тут где то надо форкаться */
            } else if (lobby.GetStatus() == Lobby::Finished) {
                lobby.Clear();
            }
        }
        has_incoming_users_.store(false);
        wait_reg_.unlock();
    }
}