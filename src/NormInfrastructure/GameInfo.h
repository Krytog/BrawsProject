#pragma once

#include <boost/asio.hpp>
#include <cstdint>

#define STR(EXPR) STR_IMPL(EXPR)
#define STR_IMPL(EXPR) #EXPR

#define GAME_HOST "localhost"
#define GAME_PORT 10000

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

enum RequestType: uint32_t {
    ConnectToGame = 1,
    CreateNewGame = 2,
    LeaveGame = 3
};

enum Character {
    MAGE,
    TANK,
    PIRATE
};

struct Player {
    uint64_t id;
    udp::endpoint endpoint;
    Character character;
};

struct Request {
    RequestType type;
    uint64_t id;
    Character character_type;
};

struct GameSettings {
    uint64_t users_count;
};