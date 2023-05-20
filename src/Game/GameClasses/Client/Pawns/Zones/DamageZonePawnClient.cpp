#include "DamageZonePawnClient.h"

#include <Core/ClientEngine.h>
#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    ROTATION = 0
};

DamageZonePawnClient::DamageZonePawnClient() {
    width_ = 0;
    height_ = 0;
    tag_ = TAGS_ZONES_HAZARD;
}

DamageZonePawnClient::~DamageZonePawnClient() = default;

void DamageZonePawnClient::UpdateSize(double width, double height) {
    if (width_ == width && height_ == height) {
        return;
    }
    width_ = width;
    height_ = height;

    // This is a bad practice, but it's too convenient here
    if (collider_) {
        CollisionSystem::GetInstance().UnregisterColliderOf(this);
    }
    collider_ = std::make_unique<RectangleCollider>(*position_, width_, height_, Collider::Category::Trigger);
    CollisionSystem::GetInstance().RegisterColliderOf(this, collider_.get());

    if (visible_object_) {
        Render::GetInstance().RemoveFromRender(this);
    }
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_ZONES_DAMAGE, *position_, width_, height_, ROTATION, IVisibleObject::RenderLayers::Effects_extra);
    Render::GetInstance().AddToRender(this, visible_object_.get());
    // End of bad practice
}

const size_t DamageZonePawnClient::kTypeId = TypeId_Zone_Damage;