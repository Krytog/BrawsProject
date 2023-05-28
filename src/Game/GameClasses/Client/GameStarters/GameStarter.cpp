#include "GameStarter.h"
#include <string>
#include <unistd.h>

#define BINARY_NAME "./Client_starter"
#define DUPER_NAME "./Dup.o"

void GameStarter::StartManualGame(uint16_t port, uint64_t id) {
    execlp(BINARY_NAME, BINARY_NAME, std::to_string(port).data(), std::to_string(id).data(), "0", NULL);
}
void GameStarter::StartBotGame(uint16_t port, uint64_t id, std::string_view bot_path) {
    execlp(DUPER_NAME, DUPER_NAME, bot_path.data(), BINARY_NAME, std::to_string(port).data(), std::to_string(id).data(), "1", NULL);
}

