#pragma once

#include "Character.h"
#include "Zelensky.h"


class Krip : public Character {
public:
    Krip(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr,
         std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const CharacterInitArgPack& char_pack, const double speed, size_t* deaths_counter,
         const GameObject* obj_to_follow);

    void OnUpdate() override;
    void Shoot(const Position &aim_pos) override;

private:
    const GameObject* obj_to_follow_;
    double speed_;
    size_t* deaths_counter_;
};


