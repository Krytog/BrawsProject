#include "ServerGameManagement.h"

#include <Core/Engine.h>
#include <Game/GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.h>

#define CRINGE "LET THE FUN BEGIN"

void ServerGameManagement::InitGameServer() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<MapsDefaultServer>();
    while (true) {
        std::string input;
        std::cin >> input;
        if (input == CRINGE) {
            break;
        }
    }
}
