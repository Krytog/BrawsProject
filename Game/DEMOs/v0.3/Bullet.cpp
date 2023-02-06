#include "Bullet.h"
#include "Character.h"

#define FLY "fly"
#define CLASH "clash"

#define SOME_BORDER 1000

Bullet::Bullet(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const double speed, const double damage, const uint64_t life_time, const Position& fly_to):
GameObject(pos_ptr, coll_ptr, vis_ptr, "bullet"), speed_(speed), damage_(damage) {
    // Event for automatic destroying character
    engine_->CreateEvent( &Bullet::ReadyToDestroy, std::tuple(this), engine_, &Engine::Destroy, std::make_tuple(this), EventStatus::Disposable);
    direction_ = fly_to.GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
    direction_.Normalize();

    IAnimated::CommonAnimationPack anim_pack;
    {
        TempStaticSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/fireball.png";
        pack.width = 50;
        pack.height = 50;
        pack.render_level = LEVELS::SECOND_USER_LEVEL;
        anim_pack[FLY] = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/BulletClash.png";
        pack.width = 100;
        pack.height = 100;
        pack.render_level = LEVELS::SECOND_USER_LEVEL;
        pack.ticks_per_frame = 2;
        pack.interrupt_points = {0};
        pack.cycled = false;
        pack.columns = 2;
        pack.rows = 2;
        anim_pack[CLASH] = pack;
    }

    IAnimated::InterruptPoints interrupt_points = {FLY, CLASH};
    IAnimated::AnimationsInitialization(anim_pack, interrupt_points);
}

void Bullet::OnUpdate() {
    if (!alive_) {
        return;
    }
    BordersCheck();
    ApplyDamage();
    Translate(direction_ * speed_);
}

void Bullet::ApplyDamage() {
    std::vector<std::string_view> active_tags = {"officer"};
    for (const auto& tag: active_tags) {
        auto collisions = engine_->GetAllCollisionsWithTag(this, tag);
        for (auto& collision: collisions) {
            if (auto object = dynamic_cast<Character*>(collision.game_object)) {
                object->ReceiveDamage(damage_);
            }
        }
        if (!collisions.empty()) {
            Die();
        }
    }
}

bool Bullet::ReadyToDestroy(const Bullet* ptr) {
    return !ptr->alive_ && ptr->visible_object_->IsFinished();
}

void Bullet::BordersCheck() {
    if (std::max(abs(position_->GetCoordinates().first), abs(position_->GetCoordinates().second)) > SOME_BORDER) {
        Die();
    };
}

void Bullet::AddAnimationDependences() {
    if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
        sequencer->AddEndTag(CLASH);
    }
}

void Bullet::Die() {
    alive_ = false;
    if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
        sequencer->SwitchAnimationTo(CLASH, AnimationSequencer::MIXED);
    } else {
        throw std::runtime_error("Character: Something weird happened to AnimationSequencer");
    }
}