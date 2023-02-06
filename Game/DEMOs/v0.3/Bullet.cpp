#include "Bullet.h"
#include "Character.h"

#define FLY "fly"
#define CLASH "clash"

#define SOME_BORDER 1000

Bullet::Bullet(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const double speed, const double damage, const uint64_t life_time, const Position& fly_to):
GameObject(pos_ptr, coll_ptr, vis_ptr, "bullet"), speed_(speed), damage_(damage) {
    engine_->CreateEvent( &Bullet::ReadyToDestroy, std::tuple(this), engine_, &Engine::Destroy, std::make_tuple(this), EventStatus::Disposable);
    direction_ = fly_to.GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
    direction_.Normalize();
}

void Bullet::OnUpdate() {
    BordersCheck();
    ApplyDamage();
    Translate(direction_ * speed_);
}

void Bullet::ApplyDamage() {
    std::vector<std::string_view> active_tags = {"officer"};
    for (const auto& tag: active_tags) {
        auto collisions = engine_->GetAllCollisionsWithTag(this, tag);
        for (auto& collision: collisions) {
            if (auto object = dynamic_cast<Character*>(collision.game_object)) {
                object->ReceiveDamage(damage_);
            }
        }
        alive_ = (collisions.empty()) && (alive_);
    }
}

bool Bullet::ReadyToDestroy(const Bullet* ptr) {
    return !ptr->alive_;
}

void Bullet::BordersCheck() {
    if (std::max(abs(position_->GetCoordinates().first), abs(position_->GetCoordinates().second)) > SOME_BORDER) {
        alive_ = false;
    };
}