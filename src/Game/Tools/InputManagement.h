#pragma once

#include <string>

namespace InputManagement {
    std::string SerializeInput();
    void HandleInput(uint64_t player_id, std::string_view input);
}