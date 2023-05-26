#pragma once
#include <Core/VisibleGameObject.h>
#include <Core/Vector2D.h>


class PowerUpPawnClient : public PersistentVisibleObject {
public:
    PowerUpPawnClient();
    virtual ~PowerUpPawnClient();

    void UpdateRotation(const Vector2D& rotator);
};
