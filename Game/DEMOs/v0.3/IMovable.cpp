#include "IMovable.h"

#include <iostream>

void IMovable::Move(const Vector2D &direction) {
    // Trying to go exactly the given direction
    collider_->Translate(direction);

    {
        auto out = engine_->GetPhysicalCollisions(this);
        collider_->Translate(direction * -1);
        if (out.empty()) {
            Translate(direction);
            return;
        }
    }

    //We fail to go exactly the given direction, but maybe we can slide along
    const double up = direction * Vector2D::Up;
    const double down = direction * Vector2D::Down;
    const double right = direction * Vector2D::Right;
    const double left = direction * Vector2D::Left;
    const double max_projection = std::max({up, down, right, left});

    Vector2D best_approx;
    if (max_projection == up) {
        best_approx = Vector2D::Up * max_projection;
    } else if (max_projection == down) {
        best_approx = Vector2D::Down * max_projection;
    } else if (max_projection == right) {
        best_approx = Vector2D::Right * max_projection;
    } else if (max_projection == left) {
        best_approx = Vector2D::Left * max_projection;
    }

    collider_->Translate(best_approx);
    {
        auto out = engine_->GetPhysicalCollisions(this);
        collider_->Translate(best_approx * -1);
        if (out.empty()) {
            Translate(best_approx);
        }
    }
}