#include "Colliders.h"

#include <stdexcept>

void CircleCollider::UpdatePosition(const Position &position) {
  position_ = position;
}

void CircleCollider::Translate(const Vector2D &vector2D) {
  position_.Translate(vector2D);
}

CircleCollider::CircleCollider(const Position &position, double radius,
                               bool is_trigger)
    : position_(position), radius_(radius) {
  is_trigger_ = is_trigger;
}

bool CircleCollider::Check(const Collider *other) const {
  if (const CircleCollider *ptr = dynamic_cast<const CircleCollider *>(other)) {
    // magic(circle, circle)
  }
  if (const RectangleCollider *ptr =
          dynamic_cast<const RectangleCollider *>(other)) {
    // magic(circle, rectangle)
  }
  throw std::runtime_error("CircleCollider::Check failed");
  return false;
}

RectangleCollider::RectangleCollider(const Position &position, double width,
                                     double height, bool is_trigger)
    : position_(position), width_(width), height_(height) {
  is_trigger_ = is_trigger;
}

void RectangleCollider::UpdatePosition(const Position &position) {
  position_ = position;
}

void RectangleCollider::Translate(const Vector2D &vector2D) {
  position_.Translate(vector2D);
}

bool RectangleCollider::Check(const Collider *other) const {
  if (const CircleCollider *ptr = dynamic_cast<const CircleCollider *>(other)) {
    // magic(rectangle, circle)
  }
  if (const RectangleCollider *ptr =
          dynamic_cast<const RectangleCollider *>(other)) {
    // magic(rectangle, rectangle)
  }
  throw std::runtime_error("RectangleCollider::Check failed");
  return false;
}