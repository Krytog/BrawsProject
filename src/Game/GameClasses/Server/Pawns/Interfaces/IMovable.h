#pragma once

#include "Core/ServerEngine.h"

class IMovable : public GameObject {
public:
    virtual void Move(const Vector2D& direction);
};