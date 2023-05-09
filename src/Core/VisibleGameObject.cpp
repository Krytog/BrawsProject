#include "VisibleGameObject.h"

VisibleGameObject::VisibleGameObject() : GameObject(), visible_object_(nullptr) {}

VisibleGameObject::VisibleGameObject(Position* position, Collider* collider, IVisibleObject* visible_object,
                       std::string_view tag)
    : GameObject(position, collider, tag),
      visible_object_(std::unique_ptr<IVisibleObject>(visible_object))
{}

VisibleGameObject::VisibleGameObject(const VisibleGameObject::ArgPack& pack)
    : GameObject(pack.position, pack.collider, pack.tag),
      visible_object_(std::unique_ptr<IVisibleObject>(pack.visible_object)) {
}

void VisibleGameObject::Translate(const Vector2D& vector2D) {
    position_->Translate(vector2D);
    if (collider_) {
        collider_->Translate(vector2D);
    }
    if (visible_object_) {
        visible_object_->Translate(vector2D);
    }
}

void VisibleGameObject::UpdatePosition(const Position& position) {
    position_->operator=(position);
    if (collider_) {
        collider_->UpdatePosition(position);
    }
    if (visible_object_) {
        visible_object_->UpdatePosition(position);
    }
}

IVisibleObject* VisibleGameObject::GetPointerToVisibleObject() const {
    return visible_object_.get();
}

// Special Persistent VisibleGameObject

PersistentVisibleObject::PersistentVisibleObject() = default;

PersistentVisibleObject::PersistentVisibleObject(Position* position, Collider* collider, IVisibleObject* visible_object,
                                   std::string_view tag)
    : VisibleGameObject(position, collider, visible_object, tag) {
}

PersistentVisibleObject::PersistentVisibleObject(const VisibleGameObject::ArgPack& pack) : VisibleGameObject(pack) {
}

void PersistentVisibleObject::OnUpdate() {
    /*   ¯\_(ツ)_/¯   */
}
