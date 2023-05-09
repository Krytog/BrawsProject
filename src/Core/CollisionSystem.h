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
        CollisionInfo(GameObject* game_object, const std::string_view tag, const Position& position,
                      Collider::Category category);

        GameObject* game_object;
        const std::string_view tag;
        Position position;
        Collider::Category category;
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
                                                std::string_view string) const;
    template <typename T>
    CollisionsInfoArray GetAllCollisionsWithType(GameObject* game_object) const {
        if (!registered_colliders_.contains(game_object)) {
            return {};
        }
        std::vector<CollisionInfo> output;
        for (const auto& [object, collider] : registered_colliders_) {
            if (!dynamic_cast<T*>(object) || collider->IsTechnical()) {
                continue;
            }
            if (object == game_object) {
                continue;
            }
            if (auto position = registered_colliders_.at(game_object)->GetIntersectionPosition(collider)) {
                output.emplace_back(object, object->GetTag(), position.value(), collider->GetCategory());
            }
        }
        return output;
    }

private:
    CollisionSystem();
    CollisionSystem(const CollisionSystem&) = delete;
    CollisionSystem& operator=(const CollisionSystem&) = delete;

    std::unordered_map<GameObject*, const Collider*> registered_colliders_;
};
