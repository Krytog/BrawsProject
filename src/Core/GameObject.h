#pragma once

#include "VisibleObject.h"
#include "Position.h"
#include "Collider.h"

#include <memory>
#include <string_view>

class GameObject {
public:
    struct ArgPack {
        Position* position;
        Collider* collider;
        VisibleObject* visible_object;
        std::string_view tag;
    };

    GameObject();
    GameObject(Position* position, Collider* collider, VisibleObject* visible_object, std::string_view tag);
    GameObject(const ArgPack& pack);

    virtual void UpdatePosition(const Position& position);
    virtual void Translate(const Vector2D& vector2D);

    Position GetPosition() const;
    const std::string_view GetTag() const;

    virtual ~GameObject() = default;

    Position* GetPointerToPosition() const;
    Collider* GetPointerToCollider() const;
    VisibleObject* GetPointerToVisibleObject() const;

    virtual void OnUpdate() = 0;

protected:
    std::unique_ptr<Position> position_;
    std::unique_ptr<Collider> collider_;
    std::unique_ptr<VisibleObject> visible_object_;
    std::string_view tag_;
};

// Special Persistent GameObject

class PersistentObject : public GameObject {
public:
    PersistentObject();
    PersistentObject(Position* position, Collider* collider, VisibleObject* visible_object,
                     std::string_view tag);
    PersistentObject(const ArgPack& pack);
    void OnUpdate() override;
};
