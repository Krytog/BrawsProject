#include "Observer.h"
#include "Engine.h"

Observer::Observer()
    : GameObject(ArgPack{.position = new Position(),
                         .collider = new RectangleCollider(Position(), kWindowWidth, kWindowHeight, true)
                         /* tag = ??? */}) {
}

void Observer::Observe(GameObject* object_to_follow) {
    object_to_follow_ = object_to_follow;
    if (object_to_follow_) {
        UpdatePosition(object_to_follow_->GetPosition());
    }
}

CollisionSystem::CollisionsInfoArray Observer::GetScreenCollisions() {
    return Engine::GetInstance().GetAllCollisions(object_to_follow_);
}

void Observer::OnUpdate() {
    if (object_to_follow_) {
        UpdatePosition(object_to_follow_->GetPosition());
    }
}
