#include "Collider.h"

bool Collider::CheckCollision(const Collider* other) const {
    return other->category_ != Trigger && Check(other);
}

bool Collider::CheckTrigger(const Collider* other) const {
    return other->category_ == Trigger && Check(other);
}

Collider::Category Collider::GetCategory() const {
    return category_;
}

bool Collider::IsOrdinary() const {
    return category_ == Ordinary;
}

bool Collider::IsTrigger() const {
    return category_ == Trigger;
}

bool Collider::IsTechnical() const {
    return category_ == Technical;
}