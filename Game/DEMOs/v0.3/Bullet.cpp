#include "Bullet.h"

Bullet::Bullet(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr,
               std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const double speed,
               const double damage, const uint64_t life_time, const Position& fly_to)
    : GameObject(pos_ptr, coll_ptr, vis_ptr, "bullet"), speed_(speed), damage_(damage) {
    engine_->Invoke(std::chrono::milliseconds(life_time), engine_, &Engine::Destroy, this);
    direction_ = fly_to.GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
    direction_.Normalize();
}

void Bullet::OnUpdate() {
    Translate(direction_ * speed_);
}