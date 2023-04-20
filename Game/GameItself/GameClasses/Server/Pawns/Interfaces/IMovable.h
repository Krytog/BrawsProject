#pragma once

#include "../../../../../Core/Engine.h"

class IMovable : public virtual GameObject {
public:
    void Move(const Vector2D& direction);
};