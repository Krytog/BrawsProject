#include "ProjectileMagePawnClient.h"
#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>

#include <Core/ClientEngine.h>

enum {
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

ProjectileMagePawnClient::ProjectileMagePawnClient() {
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_PROJECTILES_MAGE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, 0, IVisibleObject::RenderLayers::Projectiles);
    tag_ = TAGS_PROJECTILE_MAGE;
}

ProjectileMagePawnClient::~ProjectileMagePawnClient() {}

const size_t ProjectileMagePawnClient::kTypeId = TypeId_Projectile_Mage;