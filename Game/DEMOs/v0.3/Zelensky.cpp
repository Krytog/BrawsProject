#include "Zelensky.h"

#include <iostream>

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

Zelensky::Zelensky(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag, const CharacterInitArgPack &char_pack, GameObject* aim, const double speed):
                                                                                          GameObject(pos_ptr, coll_ptr, vis_ptr, tag), Character(char_pack), aim_(aim), speed_(speed) {
    std::cout << "DAROVENKO" << std::endl;
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
    Move(vector * speed_);
}