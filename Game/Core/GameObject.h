#pragma once

#include "VisibleObject.h"
#include "Position.h"
#include "Collider.h"

#include <memory>
#include <string_view>

class GameObject {
public:
    GameObject();
    GameObject(Position* position, Collider* collider,
               VisibleObject* visible_object, const std::string_view& tag);

    virtual void UpdatePosition(const Position& position);
    virtual void Translate(const Vector2D& vector2D);

    Position GetPosition() const;
    const std::string_view GetTag() const;

    virtual ~GameObject() = default;

    Position* GetPointerToPosition() const;
    Collider* GetPointerToCollider() const;
    VisibleObject* GetPointerToVisibleObject() const;

protected:
    std::unique_ptr<Position> position_;
    std::unique_ptr<Collider> collider_;
    std::unique_ptr<VisibleObject> visible_object_;
    std::string_view tag_;
};
