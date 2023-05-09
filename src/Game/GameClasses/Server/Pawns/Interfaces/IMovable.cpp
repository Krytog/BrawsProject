#include "IMovable.h"

#include <Game/GameClasses/GameObjectTags.h>

void IMovable::Move(const Vector2D &direction) {
    Vector2D projections[4];
    projections[0] = Vector2D::Up * (direction * Vector2D::Up);
    projections[1] = Vector2D::Down * (direction * Vector2D::Down);
    projections[2] = Vector2D::Right * (direction * Vector2D::Right);
    projections[3] = Vector2D::Left * (direction * Vector2D::Left);
    for (int i = 0; i < 4; ++i) {
        if (projections[i].IsZero()) {
            continue;
        }
        collider_->Translate(projections[i]);
        auto collisions = ServerEngine::GetInstance().GetPhysicalCollisions(this);
        collider_->Translate(projections[i] * -1);
        if (collisions.empty()) {
            Translate(projections[i]);
        }
    }
}