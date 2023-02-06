#pragma once

#include "../../Core/CustomBehaviour.h"
#include "IMovable.h"
#include "IAnimated.h"

#define STAND_LEFT "stand_left"
#define STAND_RIGHT "stand_right"
#define RUN_LEFT "run_left"
#define RUN_RIGHT "run_right"
#define DEATH_LEFT "death_left"
#define DEATH_RIGHT "death_right"

class Character : public virtual CustomBehaviour, public virtual GameObject, public IMovable, public IAnimated {
public:
    static bool ReadyToDestroy(const Character* ptr);

    enum MovingDirection {
        LEFT,
        RIGHT
    };

    Character(const double start_health, const Position& shoot_pos, const double damage);

    virtual void Shoot(const Position& aim_pos) = 0;

    double GetHealth() const;
    void ReceiveDamage(const double damage_taken);
    void Die();

    void CharacterMove(const Vector2D& direction);

    virtual void OnUpdate() override = 0;
    virtual void AddAnimationDependences() override;

    virtual ~Character() = default;

protected:
    MovingDirection moving_direction_;
    double health_;
    Position shoot_pos_;
    double damage_;
    bool alive_ = true;
};
