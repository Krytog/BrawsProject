#include "Collider.h"

bool Collider::CheckCollision(const Collider *other) const {
    return !other->is_trigger_ && Check(other);
}

bool Collider::CheckTrigger(const Collider *other) const {
    return other->is_trigger_ && Check(other);
}

bool Collider::IsTrigger() const {
    return is_trigger_;
}