#pragma once
#include <Core/VisibleGameObject.h>
#include <Core/Vector2D.h>

class ProjectilePawnClient : public VisibleGameObject {
public:
    ProjectilePawnClient();
    virtual ~ProjectilePawnClient();

    void UpdateRotation(const Vector2D& rotator);
    void OnUpdate() override;

protected:
    double start_rotation; // by default, we think that projectile is flying according to Vector2D::RIGHT
};

