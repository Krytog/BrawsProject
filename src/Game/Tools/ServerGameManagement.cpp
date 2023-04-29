#include "ServerGameManagement.h"

#include <Core/Engine.h>
#include <Game/GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.h>
#include <Game/GameClasses/Server/Pawns/Characters/Default/CharacterDefaultPawnServer.h>

void ServerGameManagement::InitGameServer() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<MapsDefaultServer>();
    engine->CreateGameObjectByDefault<CharacterDefaultPawnServer>();
}
