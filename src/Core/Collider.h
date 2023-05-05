#pragma once

#include "Position.h"

#include <optional>
#include <cstdint>

class Collider {
public:
    /// Collider Category\n
    /// Technical Colliders are undetectable
    enum Category : uint8_t {
        Ordinary = 0,
        Trigger = 1,
        Technical = 2,
    };

    Collider() = default;

    virtual void UpdatePosition(const Position& position) = 0;
    virtual void Translate(const Vector2D& vector2D) = 0;

    bool CheckCollision(const Collider* other) const;
    bool CheckTrigger(const Collider* other) const;
    virtual bool IsInside(const Position& position) const = 0;
    virtual std::optional<Position> GetIntersectionPosition(const Collider* other) const = 0;

    Category GetCategory() const;
    bool IsOrdinary() const;
    bool IsTrigger() const;
    bool IsTechnical() const;

    virtual ~Collider() = default;

protected:
    virtual bool Check(const Collider* other) const = 0;

    Category category_ = Ordinary;
};
