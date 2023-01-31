#pragma once

#include "../../Core/CustomBehaviour.h"
#include "IMovable.h"

struct TempStaticSpriteArgPack {
    double width;
    double height;
    std::string_view path;
    LEVELS render_level;
};

struct TempAnimatedSpriteArgPack {
    double width;
    double height;
    std::string_view path;
    LEVELS render_level;
    uint8_t ticks_per_frame;
    uint64_t columns;
    uint64_t rows;
    std::unordered_set<size_t> interrupt_points;
    bool cycled;
};

struct CharacterAnimationArgPack {
    TempStaticSpriteArgPack stand_left_animation;
    TempStaticSpriteArgPack stand_right_animation;
    TempAnimatedSpriteArgPack run_left_animation;
    TempAnimatedSpriteArgPack run_right_animation;
    TempAnimatedSpriteArgPack death_left_animation;
    TempAnimatedSpriteArgPack death_right_animation;
};

class Character : public virtual CustomBehaviour, public virtual GameObject, public IMovable {
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

    virtual ~Character() = default;

protected:
    void AnimationsInitialization(const CharacterAnimationArgPack& arg_pack);

    MovingDirection moving_direction_;
    double health_;
    Position shoot_pos_;
    double damage_;
    bool alive_ = true;
};
