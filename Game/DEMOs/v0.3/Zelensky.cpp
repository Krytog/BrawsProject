#include "Zelensky.h"

#include <iostream>

static double k_health = 100;
static Position k_shoot_pos(0, 0);
static double k_damage = 50;
static double k_speed = 10;

Vector2D ResultVector(const InputSystem::InputTokensArray& input) {
    Vector2D output(0, 0);
    for (auto token : input) {
        auto keyboard_token = std::get<InputSystem::KeyboardToken>(token);
        if (keyboard_token.symbol == 'D') {
            output += Vector2D::Right;
        } else if (keyboard_token.symbol == 'A') {
            output += Vector2D::Left;
        } else if (keyboard_token.symbol == 'W') {
            output += Vector2D::Up;
        } else if (keyboard_token.symbol == 'S') {
            output += Vector2D::Down;
        }
    }
    output.Normalize();
    return output;
}

Zelensky::Zelensky(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, GameObject* aim):
                                                                                          GameObject(pos_ptr, coll_ptr, vis_ptr, tag), Character(k_health, k_shoot_pos, k_damage), aim_(aim), speed_(k_speed) {
    CharacterAnimationArgPack arg_pack;
    {
        TempStaticSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/ZelenskyStandLeft.png";
        pack.width = 180;
        pack.height = 220;
        pack.render_level = LEVELS::SECOND_USER_LEVEL;
        arg_pack.stand_left_animation = pack;
    }
    {
        TempStaticSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/ZelenskyStandRight.png";
        pack.width = 180;
        pack.height = 220;
        pack.render_level = LEVELS::SECOND_USER_LEVEL;
        arg_pack.stand_right_animation = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/ZelenskyRunsLeft.png";
        pack.width = 200;
        pack.height = 250;
        pack.render_level = LEVELS::SECOND_USER_LEVEL;
        pack.ticks_per_frame = 2;
        pack.interrupt_points = {0};
        pack.cycled = true;
        pack.columns = 18;
        pack.rows = 1;
        arg_pack.run_left_animation = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/ZelenskyRunsRight.png";
        pack.width = 200;
        pack.height = 250;
        pack.render_level = LEVELS::SECOND_USER_LEVEL;
        pack.ticks_per_frame = 2;
        pack.interrupt_points = {0};
        pack.cycled = true;
        pack.columns = 18;
        pack.rows = 1;
        arg_pack.run_right_animation = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/ZelenskyRunsLeft.png";
        pack.width = 200;
        pack.height = 250;
        pack.render_level = LEVELS::SECOND_USER_LEVEL;
        pack.ticks_per_frame = 2;
        pack.interrupt_points = {0};
        pack.cycled = true;
        pack.columns = 18;
        pack.rows = 1;
        arg_pack.death_left_animation = pack;
    }
    {
        TempAnimatedSpriteArgPack pack;
        pack.path = "../Game/DEMOs/v0.3/Resources/ZelenskyRunsRight.png";
        pack.width = 200;
        pack.height = 250;
        pack.render_level = LEVELS::SECOND_USER_LEVEL;
        pack.ticks_per_frame = 2;
        pack.interrupt_points = {0};
        pack.cycled = true;
        pack.columns = 18;
        pack.rows = 1;
        arg_pack.death_right_animation = pack;
    }
    AnimationsInitialization(arg_pack);
};

void Zelensky::Shoot(const Position &aim_pos) {
    engine_->ProduceObject<Bullet>(new Position(position_->GetCoordinatesAsVector2D() + shoot_pos_.GetCoordinatesAsVector2D()),
                    new CircleCollider(Position(position_->GetCoordinatesAsVector2D() + shoot_pos_.GetCoordinatesAsVector2D()), 50, true),
                    new StaticSprite(new Position(position_->GetCoordinatesAsVector2D() + shoot_pos_.GetCoordinatesAsVector2D()), 50, 50, "../Game/DEMOs/v0.3/Resources/fireball.png", LEVELS::SECOND_USER_LEVEL),
                    "bullet",
                    10, 25, 2000, aim_pos);
    std::cout << "ZELENSKY SHOOT!" << std::endl;
}

void Zelensky::OnUpdate() {
    auto input = engine_->GetInput();
    auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
    input.erase(input.begin());
    if (mouse_token.key) {
        Shoot(aim_->GetPosition());
    }
    auto vector = ResultVector(input);
    CharacterMove(vector * speed_);
}