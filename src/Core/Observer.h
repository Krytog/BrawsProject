#pragma once

#include "VisibleGameObject.h"
#include "Colliders.h"
#include "CollisionSystem.h"

class Observer : public VisibleGameObject {
public:

    struct CollisionsWithPlayer {
        CollisionSystem::CollisionsInfoArray info_array;
        int64_t player_index;
    };

    Observer();

    void Observe(GameObject* object_to_follow);
    CollisionsWithPlayer GetScreenCollisions();

    void OnUpdate() override;

private:
    GameObject* object_to_follow_ = nullptr;
};
