#pragma once

#include "VisibleGameObject.h"
#include "Colliders.h"
#include "CollisionSystem.h"

class Observer : public VisibleGameObject {
public:
    Observer();

    void Observe(GameObject* object_to_follow);
    CollisionSystem::CollisionsInfoArray GetScreenCollisions();

    void OnUpdate() override;

private:
    GameObject* object_to_follow_ = nullptr;
};
