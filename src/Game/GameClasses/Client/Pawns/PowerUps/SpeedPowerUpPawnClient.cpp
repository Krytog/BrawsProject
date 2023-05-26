#include "SpeedPowerUpPawnClient.h"

#include <Core/ClientEngine.h>
#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100,
    ANGLE = 0
};

SpeedPowerUpPawnClient::SpeedPowerUpPawnClient() {
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_POWERUP_SPEED, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, ANGLE, IVisibleObject::RenderLayers::Objects);
}

SpeedPowerUpPawnClient::~SpeedPowerUpPawnClient() = default;

const size_t SpeedPowerUpPawnClient::kTypeId = TypeId_Powerup_Speed;