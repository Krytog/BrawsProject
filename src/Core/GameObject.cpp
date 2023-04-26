#include "GameObject.h"

GameObject::GameObject()
    : position_(std::make_unique<Position>(0, 0)),
      collider_(nullptr),
      visible_object_(nullptr),
      tag_("__NULL") {
}

GameObject::GameObject(Position* position, Collider* collider,
                       VisibleObject* visible_object, std::string_view tag)
    : position_(std::unique_ptr<Position>(position)),
      collider_(std::unique_ptr<Collider>(collider)),
      visible_object_(std::unique_ptr<VisibleObject>(visible_object)),
      tag_(tag) {
}

GameObject::GameObject(const GameObject::ArgPack& pack)
    : position_(std::unique_ptr<Position>(pack.position)),
      collider_(std::unique_ptr<Collider>(pack.collider)),
      visible_object_(std::unique_ptr<VisibleObject>(pack.visible_object)),
      tag_(pack.tag) {
}

void GameObject::Translate(const Vector2D& vector2D) {
    position_->Translate(vector2D);
    if (collider_) {
        collider_->Translate(vector2D);
    }
    if (visible_object_) {
        visible_object_->Translate(vector2D);
    }
}

void GameObject::UpdatePosition(const Position& position) {
    position_->operator=(position);
    if (collider_) {
        collider_->UpdatePosition(position);
    }
    if (visible_object_) {
        visible_object_->UpdatePosition(position);
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

VisibleObject* GameObject::GetPointerToVisibleObject() const {
    return visible_object_.get();
}

// Special Persistent GameObject

void PersistentObject::OnUpdate() {
    /*   ¯\_(ツ)_/¯   */
}
