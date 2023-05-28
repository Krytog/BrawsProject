#include "ServerGameManagement.h"

#include <Core/ServerEngine.h>
#include <Game/GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.h>
#include <Game/GameClasses/Server/Pawns/Maps/LudusMap/LudusMap.h>
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

    bool AlwaysTrue(Cerebrate* from, Cerebrate* other) {
        return true;
    }

    GameObject* CreateCharacter(const Position& pos, Character character) {
        ServerEngine& engine = ServerEngine::GetInstance();
        GameObject* output = nullptr;
        switch (character) {
            case Character::MAGE: {
                output = engine.CreateGameObject<CharacterMagePawnServer>(pos);
                break;
            }
            case Character::PIRATE: {
                output = engine.CreateGameObject<CharacterPiratePawnServer>(pos);
                break;
            }
            case Character::TANK: {
                output = nullptr;
                break;
            }
        }
        return output;
    }
}

static const Position kStartPositions[2] = {Position(-2000, 0), Position(2000, 0)};

void ServerGameManagement::InitGameServer(const std::unordered_map<uint64_t, Player>& players) {
    ServerEngine& engine = ServerEngine::GetInstance();
    engine.CreateGameObjectByDefault<LudusMapServer>();
    size_t counter = 0;
    for (auto& [id, player] : players) {
        GameObject* player_pawn = CreateCharacter(kStartPositions[counter], player.character);
        Overmind::GetInstance().RegisterNewPlayer(player.id, dynamic_cast<CharacterPawnServer*>(player_pawn)->GetCerebrateId());
        ++counter;
    }
    GameRuler::GetInstance().BeginGame();
    engine.CreateGameObject<HazardZoneManager>(LudusMapServer::kWidth, LudusMapServer::kHeight, 120, 1, 20);
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
