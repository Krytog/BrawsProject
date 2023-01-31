#include "Character.h"

#define RUN_LEFT "run_left"
#define RUN_RIGHT "run_right"
#define DEATH "death"

#include <iostream>

Character::Character(const CharacterInitArgPack &arg_pack): moving_direction_(RIGHT), health_(arg_pack.start_health), shoot_pos_(arg_pack.shoot_pos),
damage_(arg_pack.damage) {
    //help me

//    if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
//        sequencer->AddByTag(RUN_RIGHT, run_right);
//        sequencer->AddByTag(RUN_LEFT, run_left);
//        sequencer->AddByTag(DEATH, death);
//    }


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
        sequencer->SwitchAnimationTo(DEATH, AnimationSequencer::MIXED);
    } else {
        throw std::runtime_error("Character: Something weird happened to AnimationSequencer");
    }
}

void Character::ReceiveDamage(const double damage_taken) {
    health_ -= damage_taken;
}