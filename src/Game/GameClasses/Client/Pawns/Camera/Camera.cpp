#include "Camera.h"

void Camera::OnUpdate() {
    if (object_to_follow_) {
        UpdatePosition(object_to_follow_->GetPosition());
    }
}

Camera::Camera(GameObject* object_to_follow) : GameObject(ArgPack{
          .position = object_to_follow->GetPointerToPosition(),
          .collider = new RectangleCollider(object_to_follow->GetPosition(), kWindowWidth, kWindowHeight, false)
          /* tag = ??? */}),
      object_to_follow_(object_to_follow) {
}

