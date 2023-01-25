#pragma once

#include "Position.h"

class VisibleObject {
public:
    VisibleObject() = default;

    virtual void UpdatePosition(const Position &position) = 0;
    virtual void Translate(const Vector2D &vector2D) = 0;

    virtual void RenderIt() = 0;

    virtual ~VisibleObject() = default;
};