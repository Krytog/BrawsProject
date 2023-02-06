#include "Character.h"

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

void Character::AddAnimationDependences() {
    if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
        sequencer->AddEndTag(DEATH_LEFT);
        sequencer->AddEndTag(DEATH_RIGHT);
    }
}