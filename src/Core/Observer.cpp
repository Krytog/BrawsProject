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

Observer::CollisionsWithPlayer Observer::GetScreenCollisions() {
    auto collisions = ClientEngine::GetInstance().GetAllCollisions(this);
    int64_t id = -1;
    for (int64_t i = 0; i < collisions.size(); ++i) {
        if (collisions[i].game_object == this->object_to_follow_) {
            id = i;
            break;
        }
    }
    return {collisions, id};
}

void Observer::OnUpdate() {
    if (object_to_follow_) {
        UpdatePosition(object_to_follow_->GetPosition());
    }
}
