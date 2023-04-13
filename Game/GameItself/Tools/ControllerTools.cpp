#include "ControllerTools.h"

Vector2D ControllerTools::ResultVector(const InputSystem::InputTokensArray& input) {
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