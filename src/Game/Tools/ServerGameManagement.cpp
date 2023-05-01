#include "ServerGameManagement.h"

#include <Core/Engine.h>
#include <Game/GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.h>
#include <Game/GameClasses/Server/Pawns/Characters/Default/CharacterDefaultPawnServer.h>
#include <SwarmSystem/Overmind.h>

void ServerGameManagement::InitGameServer() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<MapsDefaultServer>();
    auto player_pawn = engine->CreateGameObjectByDefault<CharacterDefaultPawnServer>();
    Overmind::GetInstance().RegisterNewPlayer(1, dynamic_cast<CharacterDefaultPawnServer*>(player_pawn)->GetCerebrateId());
}

void ServerGameManagement::HandleInput(uint64_t player_id, std::string_view input) {
    auto cerebrate = Overmind::GetInstance().GetPlayersCerebrate(player_id);
    if (cerebrate) {
        cerebrate->ForcePossessedExecuteCommand(input);
    }
}
