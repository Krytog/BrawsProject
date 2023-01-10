#pragma once

#include "Position.h"

#include <optional>

class Collider {
public:
  Collider() = default;

  virtual void UpdatePosition(const Position &position) = 0;
  virtual void Translate(const Vector2D &vector2D) = 0;

  bool CheckCollision(const Collider *other) const;
  bool CheckTrigger(const Collider *other) const;
  virtual std::optional<Position> GetIntersectionPosition(const Collider *other) const = 0;

  bool IsTrigger() const;

  virtual ~Collider() = default;

protected:
  virtual bool Check(const Collider *other) const = 0;

  bool is_trigger_ = false;
};
