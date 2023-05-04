#pragma once

#include "Position.h"

#include <optional>

class Collider {
public:
    enum Category : bool {
        Ordinary = true,
        Technical = false
    };

    Collider() = default;

    virtual void UpdatePosition(const Position& position) = 0;
    virtual void Translate(const Vector2D& vector2D) = 0;

    bool CheckCollision(const Collider* other) const;
    bool CheckTrigger(const Collider* other) const;
    virtual bool IsInside(const Position& position) const = 0;
    virtual std::optional<Position> GetIntersectionPosition(const Collider* other) const = 0;

    bool IsTrigger() const;

    Category GetCategory() const;

    virtual ~Collider() = default;

protected:
    virtual bool Check(const Collider* other) const = 0;

    bool is_trigger_ = false;
    Category category_ = Ordinary;
};
