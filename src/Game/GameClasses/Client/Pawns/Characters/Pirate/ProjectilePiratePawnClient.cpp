#include "ProjectilePiratePawnClient.h"
#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Client/Pawns/Particles/ExplosionParticles.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    SPRITE_WIDTH = 200,
    SPRITE_HEIGHT = 200,
    ROTATION = 0
};

#define COLLIDER_RADIUS 17.5

ProjectilePiratePawnClient::ProjectilePiratePawnClient() {
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS, Collider::Category::Trigger);
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_PROJECTILES_PIRATE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT,
                                                    ROTATION, IVisibleObject::RenderLayers::Projectiles);
    tag_ = TAGS_PROJECTILE_PIRATE;
}

ProjectilePiratePawnClient::~ProjectilePiratePawnClient() {}

const size_t ProjectilePiratePawnClient::kTypeId = TypeId_Projectile_Pirate;

enum {
    MARK_COLLIDER_RADIUS = 32,
    MARK_SPRITE_WIDTH = 64,
    MARK_SPRITE_HEIGHT = 64
};

ProjectilePiratePawnClient::Mark::Mark() {
    collider_ = std::make_unique<CircleCollider>(*position_, MARK_COLLIDER_RADIUS, Collider::Category::Trigger);
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_PROJECTILES_MARK, *position_, MARK_SPRITE_WIDTH,
                                                    MARK_SPRITE_HEIGHT, ROTATION, IVisibleObject::RenderLayers::BackGround_extra);
    tag_ = TAGS_PROJECTILE_MARK;
}

ProjectilePiratePawnClient::Mark::~Mark() = default;

void ProjectilePiratePawnClient::Mark::UpdateRotation(const Vector2D& rotator) {
    visible_object_->UpdateRotation(rotator);
}

const size_t ProjectilePiratePawnClient::Mark::kTypeId = TypeId_Projectile_Mark;