#include "ServerGameManagement.h"

#include <Core/ServerEngine.h>
#include <Game/GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.h>
#include <Game/GameClasses/Server/Pawns/Characters/Default/CharacterDefaultPawnServer.h>
#include <Game/GameClasses/Server/Pawns/Characters/Mage/CharacterMagePawnServer.h>
#include <Game/GameClasses/Server/Pawns/Characters/Pirate/CharacterPiratePawnServer.h>
#include <SwarmSystem/Overmind.h>
#include <NormInfrastructure/Server/Communicator.h>
#include <Game/GameClasses/CommandsList.h>
#include <Game/GameClasses/Server/GameRulers/GameRuler.h>
#include <Game/GameClasses/Server/Pawns/Zones/HazardZoneManager.h>



namespace {
    bool IsSeenByPlayer(Cerebrate* from, Cerebrate* other) {
        auto character_pawn = static_cast<CharacterPawnServer*>(from->GetPossessed());
        auto game_object = static_cast<GameObject*>(other->GetPossessed());
        auto collision_info = ServerEngine::GetInstance().CheckPhysicalCollision(character_pawn->GetFieldOfView(), game_object);
        if (collision_info.has_value()) {
            return true;
        }
        collision_info = ServerEngine::GetInstance().CheckTriggerCollision(character_pawn->GetFieldOfView(), game_object);
        if (collision_info.has_value()) {
            return true;
        }
        return false;
    }

    bool Kostil(Cerebrate* from, Cerebrate* other) {
        return true;
    }
}

void ServerGameManagement::InitGameServer(std::vector<uint64_t>& players_id) {
    ServerEngine& engine = ServerEngine::GetInstance();
    engine.CreateGameObjectByDefault<MapsDefaultServer>();
    for (size_t i = 0; i < players_id.size(); ++i) {
        GameObject* player_pawn;
        if (i % 2 == 0) {
            player_pawn = engine.CreateGameObject<CharacterMagePawnServer>(Position(-500, 0));
            Overmind::GetInstance().RegisterNewPlayer(players_id[i], dynamic_cast<CharacterMagePawnServer*>(player_pawn)->GetCerebrateId());
        } else {
            player_pawn = engine.CreateGameObject<CharacterPiratePawnServer>(Position(500, 0));
            Overmind::GetInstance().RegisterNewPlayer(players_id[i], dynamic_cast<CharacterPiratePawnServer*>(player_pawn)->GetCerebrateId());
        }
    }
    GameRuler::GetInstance().BeginGame();
    engine.CreateGameObject<HazardZoneManager>(4000, 2000, 60, 2, 20);
}

void ServerGameManagement::HandleInput(uint64_t player_id, std::string_view input) {
    auto cerebrate = Overmind::GetInstance().GetPlayersCerebrate(player_id);
    if (cerebrate) {
        cerebrate->ForcePossessedExecuteCommand(input);
    }
}

#define WIN_STRING "!WIN!"
#define LOSE_STRING "!LOSE!"

void ServerGameManagement::PrepareAndSendDataToClient(uint64_t player_id) {
    static std::unordered_map<uint64_t, bool> viewport_captured;
    static Overmind& overmind = Overmind::GetInstance();
    static GameRuler& gameruler = GameRuler::GetInstance();
    auto player_cerebrate = overmind.GetPlayersCerebrate(player_id);
    auto player_pawn = static_cast<CharacterPawnServer*>(player_cerebrate->GetPossessed());
    std::string data;
    if (gameruler.IsGameEnded()) {
        if (gameruler.IsPlayerAlive(player_pawn)) {
            data = WIN_STRING;
        } else {
            data = LOSE_STRING;
        }
    }
    if (!viewport_captured[player_id]) {
        std::string to_capture_viewport;
        to_capture_viewport += CharacterCommands::COMMAND_CAPTURE_VIEWPORT;
        player_cerebrate->AddCommandToBuffer(to_capture_viewport);
        viewport_captured[player_id] = false;
    }
    overmind.UpdateCerebratesInfo(player_cerebrate, IsSeenByPlayer);
    data += overmind.GetCerebratesInfoSerialized();
    Communicator::GetInstance().SendToClient(player_id, data);
}
