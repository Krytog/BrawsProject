#include "../Core/Engine.h"
#include "../Core/MyTime.h"

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

int main() {
    Engine& engine = Engine::GetInstance();
    MyTime time;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }

        std::cout << "Hello world!" << std::endl;

    }
    return 0;
}
