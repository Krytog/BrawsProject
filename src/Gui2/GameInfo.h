#include <string>

struct GameInfo {
    enum class GameType {
        CREATE,
        FIND,
        RANDOM,
    };

    enum class HeroType {
        RANDOM,
        MAGE,
        TANK,
        PIRATE,
    };

    enum class ControlType {
        REGULAR,
        BOT,
    };

    GameType gamemode = GameType::CREATE;
    HeroType gamehero = HeroType::RANDOM;
    ControlType contoltype = ControlType::REGULAR;
    std::string path;
};
