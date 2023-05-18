#pragma once

#include "Qt/PaintTemplates/VisibleObject.h"
#include "Position.h"
#include "Collider.h"
#include "GameObject.h"

#include <memory>
#include <string_view>

class VisibleGameObject : public GameObject {
public:
    struct ArgPack {
        Position* position;
        Collider* collider;
        IVisibleObject* visible_object;
        std::string_view tag;
    };

    VisibleGameObject();
    VisibleGameObject(Position* position, Collider* collider, IVisibleObject* visible_object, std::string_view tag);
    VisibleGameObject(const ArgPack& pack);

    virtual void UpdatePosition(const Position& position);
    virtual void Translate(const Vector2D& vector2D);

    virtual ~VisibleGameObject() = default;

    IVisibleObject* GetPointerToVisibleObject() const;

    virtual void OnUpdate() = 0;

protected:
    std::unique_ptr<IVisibleObject> visible_object_;
};

// Special Persistent GameObject

class PersistentVisibleObject : public VisibleGameObject {
public:
    PersistentVisibleObject();
    PersistentVisibleObject(Position* position, Collider* collider, IVisibleObject* visible_object,
                     std::string_view tag);
    PersistentVisibleObject(const ArgPack& pack);
    virtual ~PersistentVisibleObject() = default;
    void OnUpdate() override;
};
