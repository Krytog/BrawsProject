#include "HealthPowerUpPawnClient.h"

#include <Core/ClientEngine.h>
#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100,
    ANGLE = 0
};

HealthPowerUpPawnClient::HealthPowerUpPawnClient() {
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_POWERUP_HEALTH, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, ANGLE, IVisibleObject::RenderLayers::Objects);
}

HealthPowerUpPawnClient::~HealthPowerUpPawnClient() = default;

const size_t HealthPowerUpPawnClient::kTypeId = TypeId_Powerup_Health;