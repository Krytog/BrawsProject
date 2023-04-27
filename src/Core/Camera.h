#pragma once

#include "GameObject.h"

class Camera {
public:
    static Camera& GetInstance() {
        static Camera instance;
        return instance;
    }

    void SetCameraOn(const GameObject* object);

    Position GetPosition() const;

private:
    Camera() = default;

    const GameObject* object_to_follow_;
};
