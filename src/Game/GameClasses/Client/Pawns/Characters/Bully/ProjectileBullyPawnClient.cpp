#include "ProjectileBullyPawnClient.h"
#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Client/Pawns/Particles/ExplosionParticles.h>
#include <SwarmSystem/TypeIdList.h>

#include <Core/ClientEngine.h>
#include <Core/MyTime.h>

enum {
    SPRITE_WIDTH = 300,
    SPRITE_HEIGHT = 300,
    ROTATION = 0
};

#define COLLIDER_RADIUS 17.5

ProjectileBullyPawnClient::ProjectileBullyPawnClient() {
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS, Collider::Category::Trigger);
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_PROJECTILES_BULLY, *position_,
                                                       SPRITE_WIDTH, SPRITE_HEIGHT, ROTATION, IVisibleObject::RenderLayers::Projectiles);
    tag_ = TAGS_PROJECTILE_BULLY;
}

ProjectileBullyPawnClient::~ProjectileBullyPawnClient() {}

const size_t ProjectileBullyPawnClient::kTypeId = TypeId_Projectile_Bully;