#pragma once

#include "Core/Engine.h"

class IMovable : public GameObject {
public:
    void Move(const Vector2D& direction);
};