#include "ClientGameManagement.h"

#include <Core/Engine.h>
#include <Game/GameClasses/Client/Pawns/Maps/Default/MapsDefaultClient.h>

#define CRINGE "LET THE FUN BEGIN"

void ClientGameManagement::InitGameClient() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<MapsDefaultClient>();
    std::cout << CRINGE << std::endl;
}