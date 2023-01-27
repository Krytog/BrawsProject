#pragma once

#include "../../Core/CustomBehaviour.h"
#include "IMovable.h"

struct CharacterInitArgPack {
    std::string_view run_left_animation_path;
    std::string_view run_right_animation_path;
    std::string_view death_animation_path;
    double start_health;
    Position shoot_pos;
    double damage;
};

class Character : public virtual CustomBehaviour, public virtual GameObject, public IMovable {
public:
    static bool ReadyToDestroy(const Character* ptr);

    enum MovingDirection {
        LEFT,
        RIGHT
    };

    Character(const CharacterInitArgPack& arg_pack);

    virtual void Shoot(const Position& aim_pos) = 0;

    double GetHealth() const;
    void ReceiveDamage(const double damage_taken);
    void Die();


    virtual void OnUpdate() override = 0;

    virtual ~Character() = default;

protected:
    MovingDirection moving_direction_;
    double health_;
    Position shoot_pos_;
    double damage_;
    bool alive_ = true;
};
