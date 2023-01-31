#include "Camera.h"

#define DefaultPosition Position(0, 0)

void Camera::SetCameraOn(const GameObject *object) {
    object_to_follow_ = object;
}

Position Camera::GetPosition() const {
    if (object_to_follow_) {
        return object_to_follow_->GetPosition();
    }
    return DefaultPosition;
}
