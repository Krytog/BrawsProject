#include "ServerGameManagement.h"

#include <Core/ServerEngine.h>
#include <Game/GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.h>
#include <Game/GameClasses/Server/Pawns/Characters/Default/CharacterDefaultPawnServer.h>
#include <Game/GameClasses/Server/Pawns/Characters/Mage/CharacterMagePawnServer.h>
#include <SwarmSystem/Overmind.h>
#include <NormInfrastructure/Server/Communicator.h>
#include <Game/GameClasses/CommandsList.h>

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
            player_pawn = engine.CreateGameObjectByDefault<CharacterMagePawnServer>();
            Overmind::GetInstance().RegisterNewPlayer(players_id[i], dynamic_cast<CharacterMagePawnServer*>(player_pawn)->GetCerebrateId());
        } else {
            player_pawn = engine.CreateGameObjectByDefault<CharacterDefaultPawnServer>();
            Overmind::GetInstance().RegisterNewPlayer(players_id[i], dynamic_cast<CharacterDefaultPawnServer*>(player_pawn)->GetCerebrateId());
        }
    }
}

void ServerGameManagement::HandleInput(uint64_t player_id, std::string_view input) {
    auto cerebrate = Overmind::GetInstance().GetPlayersCerebrate(player_id);
    if (cerebrate) {
        cerebrate->ForcePossessedExecuteCommand(input);
    }
}

void ServerGameManagement::PrepareAndSendDataToClient(uint64_t player_id) {
    static std::unordered_map<uint64_t, bool> viewport_captured;
    Overmind& overmind = Overmind::GetInstance();
    auto player_cerebrate = overmind.GetPlayersCerebrate(player_id);
    if (!viewport_captured[player_id]) {
        std::string to_capture_viewport;
        to_capture_viewport += CharacterCommands::COMMAND_CAPTURE_VIEWPORT;
        player_cerebrate->AddCommandToBuffer(to_capture_viewport);
        viewport_captured[player_id] = true;
    }
    overmind.UpdateCerebratesInfo(player_cerebrate, IsSeenByPlayer);
    auto data = overmind.GetCerebratesInfoSerialized();
    std::cout << "DATA SIZE: " << data.size() << std::endl;
    Communicator::GetInstance().SendToClient(player_id, data);
}
