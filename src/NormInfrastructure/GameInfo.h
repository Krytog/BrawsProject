#pragma once

#include <boost/asio.hpp>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

#define STR(EXPR) STR_IMPL(EXPR)
#define STR_IMPL(EXPR) #EXPR

#define GAME_HOST "194.87.252.128"
#define GAME_PORT 10005
#define COMMUNICATOR_RECEIVE_PORT 10013

#define GAME_APPROVE "start"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

enum RequestType: uint32_t {
    ConnectToGame = 1,
    CreateNewGame = 2,
    LeaveGame = 3,
    EndGameSession = 4
};

enum Character {
    MAGE,
    TANK,
    PIRATE
};

static constexpr uint64_t kGameUndefined = 0;

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
