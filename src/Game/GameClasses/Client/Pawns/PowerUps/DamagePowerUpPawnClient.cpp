#include "DamagePowerUpPawnClient.h"

#include <Core/ClientEngine.h>
#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100,
    ANGLE = 0
};

DamagePowerUpPawnClient::DamagePowerUpPawnClient() {
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_POWERUP_DAMAGE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, ANGLE, IVisibleObject::RenderLayers::Objects);
}

DamagePowerUpPawnClient::~DamagePowerUpPawnClient() = default;

const size_t DamagePowerUpPawnClient::kTypeId = TypeId_Powerup_Damage;