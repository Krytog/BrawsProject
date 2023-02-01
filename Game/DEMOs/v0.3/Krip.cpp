#include "Krip.h"

Krip::Krip(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr,
           std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag,
           const CharacterInitArgPack& char_pack, const double speed, size_t* deaths_counter,
           const GameObject* obj_to_follow)
    : GameObject(pos_ptr, coll_ptr, vis_ptr, tag),
      Character(char_pack),
      speed_(speed),
      deaths_counter_(deaths_counter),
      obj_to_follow_(obj_to_follow) {
}

void Krip::OnUpdate() {
    std::cout << "UpDateKripStart\n";
    Vector2D direction =
        obj_to_follow_->GetPosition().GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
    direction.Normalize();
    Move(direction);
    std::cout << "UpDateKripEnd\n";
}

void Krip::Shoot(const Position& aim_pos){};