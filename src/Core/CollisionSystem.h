#pragma once

#include "GameObject.h"

#include <optional>
#include <unordered_map>
#include <vector>

class CollisionSystem {
public:
    static CollisionSystem& GetInstance();

    struct CollisionInfo {
        CollisionInfo() = default;
        CollisionInfo(GameObject* game_object_, const std::string_view tag_, const Position& position_,
                      const bool is_trigger_);

        GameObject* game_object;
        const std::string_view tag;
        Position position;
        bool is_trigger;
    };

    bool IsRegistered(GameObject* game_object) const;
    const Collider* GetRegisteredColliderOf(GameObject* game_object) const;

    void RegisterColliderOf(GameObject* game_object, const Collider* collider);
    void UnregisterColliderOf(GameObject* game_object);

    using PossiblePosition = std::optional<Position>;
    using CollisionsInfoArray = std::vector<CollisionInfo>;

    PossiblePosition CheckPhysicalCollision(GameObject* first, GameObject* second) const;
    PossiblePosition CheckTriggerCollision(GameObject* first, GameObject* second) const;

    CollisionsInfoArray GetAllCollisions(GameObject* game_object) const;
    CollisionsInfoArray GetPhysicalCollisions(GameObject* game_object) const;
    CollisionsInfoArray GetTriggerCollisions(GameObject* game_object) const;
    CollisionsInfoArray GetAllCollisionsWithTag(GameObject* game_object,
                                                const std::string_view string) const;
    template <typename T>
    CollisionsInfoArray GetAllCollisionsWithType(GameObject* game_object) const;

private:
    CollisionSystem();

    std::unordered_map<GameObject*, const Collider*> registered_colliders_;
};
