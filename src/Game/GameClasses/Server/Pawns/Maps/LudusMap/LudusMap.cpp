#include "LudusMap.h"

#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/HealthPowerUpPawnServer.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/SpeedPowerUpPawnServer.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/DamagePowerUpPawnServer.h>

enum {
    LAKE_X = 0,
    LAKE_Y = 0,
    LAKE_WIDTH = 400,
    LAKE_HEIGHT = 600,
    MAP_WIDTH = 16000,
    MAP_HEIGHT = 8000
};

LudusMapServer::LudusMapServer(): PersistentObject(new Position(0, 0), nullptr, TAGS_MAPS_Default) {
    ServerEngine::GetInstance().CreateGameObject<HealthPowerUpPawnServer>(Position(0, -800));
    ServerEngine::GetInstance().CreateGameObject<SpeedPowerUpPawnServer>(Position(200, -800));
    ServerEngine::GetInstance().CreateGameObject<DamagePowerUpPawnServer>(Position(-200, -800));
}