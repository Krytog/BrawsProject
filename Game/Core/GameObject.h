#pragma once

#include "VisibleObject.h"
#include "Position.h"
#include "Collider.h"

#include <memory>
#include <string_view>

class GameObject {
public:
    GameObject();
    GameObject(std::unique_ptr<Position> position, std::unique_ptr<Collider> collider, std::unique_ptr<VisibleObject> visible_object, const std::string_view& tag);

    virtual void UpdatePosition(const Position& position);
    virtual void Translate(const Vector2D& vector2D);

    Position GetPosition() const;
    const std::string_view GetTag() const;

    virtual ~GameObject() = default;

private:
    std::unique_ptr<Position> position_;
    std::unique_ptr<Collider> collider_;
    std::unique_ptr<VisibleObject> visible_object_;
    const std::string_view tag_;
};
