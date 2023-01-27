#pragma once

#include "../../Core/CustomBehaviour.h"

struct GameObjectArgPack {
    std::unique_ptr<Position> position;
    std::unique_ptr<Collider> collider;
    std::unique_ptr<VisibleObject> visible_object;
};

class Bullet : public virtual GameObject, public virtual CustomBehaviour {
public:
    Bullet(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const double speed, const double damage, const uint64_t life_time, const Position& fly_to);

    void OnUpdate() override;

protected:
    double speed_;
    double damage_;
    Vector2D direction_;
};

