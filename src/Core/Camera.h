#pragma once

#include "GameObject.h"

class Camera {
public:
    static Camera& GetInstance();

    void SetCameraOn(const GameObject* object);

    Position GetPosition() const;

private:
    Camera() = default;
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    const GameObject* object_to_follow_;
};
