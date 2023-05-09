#pragma once

#include "Core/ServerEngine.h"

class IMovable : public GameObject {
public:
    void Move(const Vector2D& direction);
};