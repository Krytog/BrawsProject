#include "Observer.h"
#include "ClientEngine.h"

Observer::Observer()
    : VisibleGameObject(ArgPack{.position = new Position(),
                         .collider = new RectangleCollider(Position(), FIELD_OF_VIEW_WIDTH, FIELD_OF_VIEW_HEIGHT, Collider::Technical),
                         .visible_object = nullptr  // Technical object
                         /* tag = ??? */}) {
}

void Observer::Observe(GameObject* object_to_follow) {
    object_to_follow_ = object_to_follow;
    if (object_to_follow_) {
        UpdatePosition(object_to_follow_->GetPosition());
    }
}

CollisionSystem::CollisionsInfoArray Observer::GetScreenCollisions() {
    return ClientEngine::GetInstance().GetAllCollisions(object_to_follow_);
}

void Observer::OnUpdate() {
    if (object_to_follow_) {
        UpdatePosition(object_to_follow_->GetPosition());
    }
}
