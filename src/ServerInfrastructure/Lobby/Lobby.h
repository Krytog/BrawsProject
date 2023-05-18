#pragma once
#include <_types/_uint64_t.h>
#include <boost/asio.hpp>
#include <unordered_map>

using boost::asio::ip::udp;

enum Character {
    MAGE,
    TANK,
    PIRATE
};

struct Player {
    uint64_t id;
    udp::endpoint connection;
    Character character;
};

class Lobby {
public:

    Lobby(size_t users_count);

    void AddPlayer(const Player& player);
    void RemovePlayer(uint64_t id);
    void StartLobby();
    void CLear();

    ~Lobby();
private:
    std::unordered_map<uint64_t, Player> players_;
    size_t users_count_;
};