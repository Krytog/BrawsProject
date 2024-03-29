#include "GameObject.h"

GameObject::GameObject()
    : position_(std::make_unique<Position>(0, 0)),
      collider_(nullptr),
      tag_("__NULL") {
}

GameObject::GameObject(Position* position, Collider* collider,
                       std::string_view tag)
    : position_(std::unique_ptr<Position>(position)),
      collider_(std::unique_ptr<Collider>(collider)),
      tag_(tag) {
}

GameObject::GameObject(const GameObject::ArgPack& pack)
    : position_(std::unique_ptr<Position>(pack.position)),
      collider_(std::unique_ptr<Collider>(pack.collider)),
      tag_(pack.tag) {
}

void GameObject::Translate(const Vector2D& vector2D) {
    position_->Translate(vector2D);
    if (collider_) {
        collider_->Translate(vector2D);
    }
}

void GameObject::UpdatePosition(const Position& position) {
    position_->operator=(position);
    if (collider_) {
        collider_->UpdatePosition(position);
    }
}

Position GameObject::GetPosition() const {
    return *position_;
}

const std::string_view GameObject::GetTag() const {
    return tag_;
}

Position* GameObject::GetPointerToPosition() const {
    return position_.get();
}

Collider* GameObject::GetPointerToCollider() const {
    return collider_.get();
}

// Special Persistent GameObject

PersistentObject::PersistentObject() = default;

PersistentObject::PersistentObject(Position* position, Collider* collider,
                                   std::string_view tag)
    : GameObject(position, collider, tag) {
}

PersistentObject::PersistentObject(const GameObject::ArgPack& pack) : GameObject(pack) {
}

void PersistentObject::OnUpdate() {
    /*   ¯\_(ツ)_/¯   */
}
