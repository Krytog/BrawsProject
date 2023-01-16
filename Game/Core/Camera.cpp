#include "Camera.h"

void Camera::SetCameraOn(const GameObject *object) {
    object_to_follow_ = object;
}

Position Camera::GetPosition() const {
    return object_to_follow_->GetPosition();
}
