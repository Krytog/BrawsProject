#include "Airplane.h"

/* Need to be enum class later */
#define AIR_FLY "fly"
#define AIR_STAY "stay"

#define PRECISION 1e-9

Airplane::Airplane(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr,
                   std::string_view tag, const double speed, const double damage, const GameObject* object_to_follow): GameObject(pos_ptr, coll_ptr, vis_ptr, tag),
                        speed_(speed), damage_(damage), obj_to_follow_(object_to_follow){
    // Event for automatic destroying character
    engine_->CreateEvent( &Airplane::ReadyToDestroy, std::tuple(this), engine_, &Engine::Destroy, std::make_tuple(this), EventStatus::Disposable);
    IAnimated::CommonAnimationPack anim_pack;
    {
        TempStaticSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/AirPlaneStay.png";
        pack.width = 80;
        pack.height = 80;
        pack.render_level = LEVELS::THIRD_USER_LEVEL;
        anim_pack[AIR_STAY] = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/AirPlaneFly.png";
        pack.width = 80;
        pack.height = 80;
        pack.render_level = LEVELS::THIRD_USER_LEVEL;
        pack.ticks_per_frame = 6;
        pack.interrupt_points = {0};
        pack.cycled = true;
        pack.columns = 5;
        pack.rows = 1;
        anim_pack[AIR_FLY] = pack;
    }

    IAnimated::InterruptPoints interrupt_points = {AIR_FLY, AIR_STAY};
    IAnimated::AnimationsInitialization(anim_pack, interrupt_points);
}

void Airplane::AddAnimationDependences() {
    if (auto sequencer = dynamic_cast<AnimationSequencer*>(visible_object_.get())) {
        sequencer->SetStartTag(AIR_FLY);
    } else {
        throw std::runtime_error("Character: Something weird happened to AnimationSequencer");
    }
}

void Airplane::OnUpdate() {
    if (!alive_) { return; }
    auto follow_position = obj_to_follow_->GetPosition().GetCoordinatesAsVector2D();
    follow_position += Vector2D::Up * up_shift_;

    if ( (position_->GetCoordinatesAsVector2D() - follow_position).Length() < PRECISION)
}
