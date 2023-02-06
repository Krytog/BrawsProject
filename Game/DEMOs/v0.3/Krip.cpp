#include "Krip.h"

uint8_t Krip::instance_count = 0;

static std::vector<std::string_view> k_kostil = {
        "../Game/DEMOs/v0.3/Resources/Krip0StandLeft.png",
        "../Game/DEMOs/v0.3/Resources/Krip0StandRight.png",
        "../Game/DEMOs/v0.3/Resources/Krip0RunLeft.png",
        "../Game/DEMOs/v0.3/Resources/Krip0RunRight.png",
        "../Game/DEMOs/v0.3/Resources/Krip0RunLeft.png",
        "../Game/DEMOs/v0.3/Resources/Krip0RunRight.png",
        "../Game/DEMOs/v0.3/Resources/Krip1StandLeft.png",
        "../Game/DEMOs/v0.3/Resources/Krip1StandRight.png",
        "../Game/DEMOs/v0.3/Resources/Krip1RunLeft.png",
        "../Game/DEMOs/v0.3/Resources/Krip1RunRight.png",
        "../Game/DEMOs/v0.3/Resources/Krip1RunLeft.png",
        "../Game/DEMOs/v0.3/Resources/Krip1RunRight.png"
};

Krip::Krip(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const double speed, size_t* deaths_counter, const GameObject* obj_to_follow):
      GameObject(pos_ptr, coll_ptr, vis_ptr, tag), Character(100, Position(0, 0), 25), speed_(speed), deaths_counter_(deaths_counter), obj_to_follow_(obj_to_follow) {
    ++instance_count;

    // Event to check whether object is ready to Die
     engine_->CreateEvent(&Krip::ReadyToDie, std::tuple(this), dynamic_cast<Character*>(this), &Character::Die, {},EventStatus::Disposable);

    //Spawn inside each other avoiding
    engine_->Invoke(0, this, &Krip::SpawnGlitchAvoiding);

    int type = rand() % 2;

    IAnimated::CommonAnimationPack anim_pack;
    {
        TempStaticSpriteArgPack pack;
        pack.path = k_kostil[type * 6 + 0];
        pack.width = 100;
        pack.height = 200;
        pack.render_level = LEVELS::FIRST_USER_LEVEL;
        anim_pack[STAND_LEFT] = pack;
    }
    {
        TempStaticSpriteArgPack pack;
        pack.path = k_kostil[type * 6 + 1];
        pack.width = 100;
        pack.height = 200;
        pack.render_level = LEVELS::FIRST_USER_LEVEL;
        anim_pack[STAND_RIGHT] = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path =  k_kostil[type * 6 + 2];
        pack.width = 100;
        pack.height = 200;
        pack.render_level = LEVELS::FIRST_USER_LEVEL;
        pack.ticks_per_frame = 5;
        pack.interrupt_points = {0};
        pack.cycled = true;
        pack.columns = 7;
        pack.rows = 1;
        anim_pack[RUN_LEFT] = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = k_kostil[type * 6 + 3];
        pack.width = 100;
        pack.height = 200;
        pack.render_level = LEVELS::FIRST_USER_LEVEL;
        pack.ticks_per_frame = 5;
        pack.interrupt_points = {0};
        pack.cycled = true;
        pack.columns = 7;
        pack.rows = 1;
        anim_pack[RUN_RIGHT] = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/KripDie.png";
        pack.width = 150;
        pack.height = 150;
        pack.render_level = LEVELS::FIRST_USER_LEVEL;
        pack.ticks_per_frame = 4;
        pack.interrupt_points = {0};
        pack.cycled = false;
        pack.columns = 2;
        pack.rows = 2;
        anim_pack[DEATH_LEFT] = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/KripDie.png";
        pack.width = 150;
        pack.height = 150;
        pack.render_level = LEVELS::FIRST_USER_LEVEL;
        pack.ticks_per_frame = 4;
        pack.interrupt_points = {0};
        pack.cycled = false;
        pack.columns = 2;
        pack.rows = 2;
        anim_pack[DEATH_RIGHT] = pack;
    }

    IAnimated::InterruptPoints interrupt_points = {STAND_RIGHT, STAND_LEFT, RUN_RIGHT, RUN_LEFT, DEATH_RIGHT, DEATH_LEFT};
    AnimationsInitialization(anim_pack, interrupt_points);
}

void Krip::OnUpdate() {
    if (!alive_) {
        return;
    } /* It seems to need to be added to the Character, at least otherwise the death animation gets lost,
 * since FORCE is used to animate the movements */


    Vector2D direction = obj_to_follow_->GetPosition().GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
    direction.Normalize();

    const double magic_constant = 200; // The distance from target when Krip should stop
    const double distance = (obj_to_follow_->GetPosition().GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D()).Length();

    if (distance < magic_constant) {
        CharacterMove(Vector2D(0, 0));
    } else {
        CharacterMove(direction * speed_);
    }
}

uint8_t Krip::GetInstanceCount() {
    return instance_count;
}

bool Krip::ReadyToDie(const Krip* ptr) {
    return ptr->health_ <= 0;
}

void Krip::Shoot(const Position& aim_pos) {};

Krip::~Krip() {
    --instance_count;
}

void Krip::SpawnGlitchAvoiding() {
    auto collisions = engine_->GetPhysicalCollisions(this);
    for (auto& collision : collisions) {
        Vector2D fix_direction = position_->GetCoordinatesAsVector2D() - collision.game_object->GetPosition().GetCoordinatesAsVector2D();
        fix_direction.Normalize();
        fix_direction *= 100;
        Translate(fix_direction);
    }
}