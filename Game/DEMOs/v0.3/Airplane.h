#pragma once

#include "../../Core/CustomBehaviour.h"
#include "IAnimated.h"

class Airplane : public virtual GameObject, public virtual CustomBehaviour, public IAnimated  {
public:
    Airplane(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const double speed, const double damage, const GameObject* object_to_follow);

    void OnUpdate() override;

    void Bombardment();

    static bool ReadyToDestroy(const Airplane* ptr);
    void BordersCheck();

    void AddAnimationDependences() override;
    void Die();

private:
    const GameObject* obj_to_follow_;
    double speed_;
    double damage_;
    bool alive_ = true;

    const double up_shift_ = 60;
};