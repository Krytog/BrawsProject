#pragma once

#include <string_view>
#include <vector>
#include <unordered_map>

#include <NormInfrastructure/GameInfo.h>

namespace ServerGameManagement {
    void InitGameServer(const std::unordered_map<uint64_t, Player>& players);
    void HandleInput(uint64_t player_id, std::string_view input);
    void PrepareAndSendDataToClient(uint64_t player_id);
};