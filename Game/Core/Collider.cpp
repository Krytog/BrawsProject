#include "Collider.h"

bool Collider::CheckCollision(const Collider *other) const {
  return Check(other) && !other->is_trigger_;
}

bool Collider::CheckTrigger(const Collider *other) const {
  return Check(other) && other->is_trigger_;
}