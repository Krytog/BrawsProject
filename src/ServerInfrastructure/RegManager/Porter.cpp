#include "Porter.h"

Porter::Lobby::Lobby(size_t users_count) : users_count_(users_count) {}

void Porter::Lobby::SetPlayerCount(size_t users_count) {
    users_count_ = users_count;
}

void Porter::Lobby::AddPlayer(const Player& player) {
    if (!players_.contains(player.id)) {
        std::cout << "Cannot register player twice" << std::endl;
    } else {
        players_[player.id] = player;
    }
}

bool Porter::Lobby::RemovePlayer(uint64_t id) {
    return players_.erase(id);
}

bool Porter::Lobby::Ready() const{
    return users_count_ == players_.size();
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
