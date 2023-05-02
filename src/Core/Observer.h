#pragma once

#include "GameObject.h"
#include "Colliders.h"
#include "CollisionSystem.h"

class Observer : public GameObject {
public:
    Observer();

    void Observe(GameObject* object_to_follow);
    CollisionSystem::CollisionsInfoArray GetScreenCollisions();

    void OnUpdate() override;

private:
    GameObject* object_to_follow_ = nullptr;
};
