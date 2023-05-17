#include "ProjectileMagePawnClient.h"
#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Client/Pawns/Particles/ExplosionParticles.h>
#include <SwarmSystem/TypeIdList.h>

#include <Core/ClientEngine.h>
#include <Core/MyTime.h>

enum {
    SPRITE_WIDTH = 200,
    SPRITE_HEIGHT = 200,
    TICKS = 1,
    FRAMES = 14,
    CYCLED = true,
    ROTATION = 0
};

#define COLLIDER_RADIUS 17.5

ProjectileMagePawnClient::ProjectileMagePawnClient() {
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS, Collider::Category::Trigger);
    visible_object_ = std::make_unique<AnimatedSprite>(RES_PATH_PROJECTILES_MAGE, *position_,
                                                       SPRITE_WIDTH, SPRITE_HEIGHT, TICKS, FRAMES,
                                                       CYCLED, ROTATION, IVisibleObject::RenderLayers::Projectiles);
    tag_ = TAGS_PROJECTILE_MAGE;
}

ProjectileMagePawnClient::~ProjectileMagePawnClient() {}

const size_t ProjectileMagePawnClient::kTypeId = TypeId_Projectile_Mage;