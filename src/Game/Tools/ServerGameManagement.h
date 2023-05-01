#pragma once

#include <string_view>

namespace ServerGameManagement {
    void InitGameServer();
    void HandleInput(uint64_t player_id, std::string_view input);
};