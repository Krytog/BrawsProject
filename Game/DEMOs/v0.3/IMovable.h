#pragma once

#include "../../Core/CustomBehaviour.h"


class IMovable : public virtual GameObject, public virtual CustomBehaviour {
public:
    IMovable() = default;

    virtual void Move(const Vector2D& direction);

    virtual ~IMovable() = default;
};