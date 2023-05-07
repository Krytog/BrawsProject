#include "ControllerTools.h"

Vector2D ControllerTools::ResultVector(std::string_view input) {
    Vector2D output(0, 0);
    for (auto token : input) {
        if (token == 'D') {
            output += Vector2D::Right;
        } else if (token == 'A') {
            output += Vector2D::Left;
        } else if (token == 'W') {
            output += Vector2D::Up;
        } else if (token == 'S') {
            output += Vector2D::Down;
        }
    }
    output.Normalize();
    return output;
}