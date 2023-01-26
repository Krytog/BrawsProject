#pragma once

<<<<<<< HEAD
#include "Canvas.h"
=======
>>>>>>> main
#include "Position.h"

class VisibleObject {
public:
    VisibleObject() = default;

    virtual void UpdatePosition(const Position &position) = 0;
    virtual void Translate(const Vector2D &vector2D) = 0;
    
    virtual void RenderIt(Canvas* canvas) const = 0;
    virtual size_t GetRenderLevel() const = 0;

    virtual bool IsFinished() const = 0;
    virtual bool IsAbleToInterrupt() const = 0;
//    virtual void ResetAnimationOptions() = 0;

    virtual ~VisibleObject() = default;
};