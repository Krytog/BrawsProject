#include "Character.h"

#define STAND_LEFT "stand_left"
#define STAND_RIGHT "stand_right"
#define RUN_LEFT "run_left"
#define RUN_RIGHT "run_right"
#define DEATH_LEFT "death_left"
#define DEATH_RIGHT "death_right"
#define PRECISION 1e-9

#include <iostream>

Character::Character(const double start_health, const Position& shoot_pos, const double damage): moving_direction_(LEFT), health_(start_health), shoot_pos_(shoot_pos),
damage_(damage) {
    // Event for automatic destroying character
    engine_->CreateEvent(&Character::ReadyToDestroy, std::tuple(this), engine_, &Engine::Destroy, std::tuple(this), EventStatus::Disposable);
}

bool Character::ReadyToDestroy(const Character *ptr) {
    return !ptr->alive_ && ptr->visible_object_->IsFinished();
}

double Character::GetHealth() const {
    return health_;
}

void Character::Die() {
    alive_ = false;
    if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
        if (moving_direction_ == MovingDirection::RIGHT) {
            sequencer->SwitchAnimationTo(DEATH_RIGHT, AnimationSequencer::MIXED);
        } else {
            sequencer->SwitchAnimationTo(DEATH_LEFT, AnimationSequencer::MIXED);
        }
    } else {
        throw std::runtime_error("Character: Something weird happened to AnimationSequencer");
    }
}

void Character::ReceiveDamage(const double damage_taken) {
    health_ -= damage_taken;
}

void Character::CharacterMove(const Vector2D &direction) {
    if (direction.Length() < PRECISION) {
        if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
            std::string_view tag = moving_direction_ == LEFT ? STAND_LEFT : STAND_RIGHT;
            sequencer->SwitchAnimationTo(tag, AnimationSequencer::FORCE);
            return;
        }
    }
    const double left = Vector2D::Left * direction;
    const double right = Vector2D::Right * direction;
    const double max_projection = std::max(left, right);
    MovingDirection new_moving_direction;
    if (left == max_projection) {
        new_moving_direction = LEFT;
    } else {
        new_moving_direction = RIGHT;
    }
    if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
        std::string_view tag;
        if (max_projection > PRECISION) {
            tag = new_moving_direction == LEFT ? RUN_LEFT : RUN_RIGHT;
            moving_direction_ = new_moving_direction;
        } else {
            tag = moving_direction_ == LEFT ? RUN_LEFT : RUN_RIGHT;
        }
        sequencer->SwitchAnimationTo(tag, AnimationSequencer::FORCE);
    } else {
        throw std::runtime_error("Character: something weird happened to animation sequencer");
    }
    Move(direction);
}

void Character::AnimationsInitialization(const CharacterAnimationArgPack &arg_pack) {
    std::vector<std::pair<std::string_view, VisibleObject*>> decoy_vector;
    std::unordered_set<std::string_view> interrupt_points = {STAND_RIGHT, STAND_LEFT, RUN_RIGHT, RUN_LEFT, DEATH_RIGHT, DEATH_LEFT};

    VisibleObject* stand_left;
    VisibleObject* stand_right;
    VisibleObject* run_left;
    VisibleObject* run_right;
    VisibleObject* death_left;
    VisibleObject* death_right;

    {
        auto anim = arg_pack.stand_left_animation;
        stand_left = new StaticSprite(position_.get(), anim.width, anim.height, anim.path, anim.render_level);
        anim = arg_pack.stand_right_animation;
        stand_right = new StaticSprite(position_.get(), anim.width, anim.height, anim.path, anim.render_level);
    }

    {
        auto anim = arg_pack.run_left_animation;
        run_left = new AnimatedSprite(position_.get(), anim.width, anim.height, anim.path, anim.render_level, anim.ticks_per_frame, anim.columns, anim.rows,
                                      anim.interrupt_points, anim.cycled);
        anim = arg_pack.run_right_animation;
        run_right = new AnimatedSprite(position_.get(), anim.width, anim.height, anim.path, anim.render_level, anim.ticks_per_frame, anim.columns, anim.rows,
                                      anim.interrupt_points, anim.cycled);
        anim = arg_pack.death_left_animation;
        death_left = new AnimatedSprite(position_.get(), anim.width, anim.height, anim.path, anim.render_level, anim.ticks_per_frame, anim.columns, anim.rows,
                                      anim.interrupt_points, anim.cycled);
        anim = arg_pack.death_right_animation;
        death_right = new AnimatedSprite(position_.get(), anim.width, anim.height, anim.path, anim.render_level, anim.ticks_per_frame, anim.columns, anim.rows,
                                      anim.interrupt_points, anim.cycled);
    }

    decoy_vector.emplace_back(RUN_LEFT, run_left);
    decoy_vector.emplace_back(RUN_RIGHT, run_right);
    decoy_vector.emplace_back(DEATH_LEFT, death_left);
    decoy_vector.emplace_back(DEATH_RIGHT, death_right);


    if (visible_object_.get()) {
        throw std::runtime_error("Character: visible object has been already initialized!");
    }

    visible_object_ = std::make_unique<AnimationSequencer>(decoy_vector, interrupt_points, true);

    if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
        sequencer->AddByTag(STAND_LEFT, stand_left);
        sequencer->AddByTag(STAND_RIGHT, stand_right);

        sequencer->AddEndTag(DEATH_LEFT);
        sequencer->AddEndTag(DEATH_RIGHT);
    }

    engine_->RenderSwith(this, visible_object_.get());
}