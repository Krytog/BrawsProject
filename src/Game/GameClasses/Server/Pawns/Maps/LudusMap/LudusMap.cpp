#include "LudusMap.h"

#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/HealthPowerUpPawnServer.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/SpeedPowerUpPawnServer.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/DamagePowerUpPawnServer.h>

enum {
    LAKE_X = 0,
    LAKE_Y = 0,
    MAP_WIDTH = 16000,
    MAP_HEIGHT = 8667,
};

LudusMapServer::LudusMapServer(): PersistentObject(new Position(0, 0), nullptr, TAGS_MAPS_Ludus) {
    ServerEngine::GetInstance().CreateGameObject<HealthPowerUpPawnServer>(Position(0, -800));
    ServerEngine::GetInstance().CreateGameObject<SpeedPowerUpPawnServer>(Position(200, -800));
    ServerEngine::GetInstance().CreateGameObject<DamagePowerUpPawnServer>(Position(-200, -800));
}

const size_t LudusMapServer::kHeight = MAP_HEIGHT / 2;
const size_t LudusMapServer::kWidth = MAP_WIDTH / 2;