#pragma once

#include <string_view>
#include <vector>

namespace ServerGameManagement {
    void InitGameServer(std::vector<uint64_t>& players_id);
    void HandleInput(uint64_t player_id, std::string_view input);
    void PrepareAndSendDataToClient(uint64_t player_id);
};