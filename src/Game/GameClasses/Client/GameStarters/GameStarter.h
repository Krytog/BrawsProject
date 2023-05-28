#pragma once
#include <stdint.h>
#include <string>

namespace GameStarter {
    void StartManualGame(uint16_t port, uint64_t id);
    void StartBotGame(uint16_t port, uint64_t id, std::string_view bot_path);
}