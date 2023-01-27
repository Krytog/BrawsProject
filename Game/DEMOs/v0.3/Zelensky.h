#pragma once

#include "Character.h"
#include "Bullet.h"

class Zelensky : public Character {
public:
    Zelensky(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const CharacterInitArgPack& char_pack, GameObject* aim, const double speed);

    void Shoot(const Position& aim_pos) override;
    void OnUpdate() override;
private:
    GameObject* aim_;
    double speed_;
};

