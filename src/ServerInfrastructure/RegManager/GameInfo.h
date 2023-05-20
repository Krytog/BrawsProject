#include <boost/asio.hpp>
#include <cstdint>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

enum RequestType: uint32_t {
    ConnectToGame = 1,
    CreateNewGame = 2
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