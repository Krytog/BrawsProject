#pragma once
#include <Core/VisibleGameObject.h>
#include <Core/Vector2D.h>

class ProjectilePawnClient : public VisibleGameObject {
public:
    ProjectilePawnClient();
    virtual ~ProjectilePawnClient();

    void UpdateRotation(const Vector2D& rotator);
    void OnUpdate() override;
};

