#pragma once

#include "GameObject.h"

#include <optional>
#include <unordered_map>
#include <vector>

class CollisionSystem {
public:
    struct CollisionInfo {
        CollisionInfo() = default;
        CollisionInfo(const GameObject* game_object_, const std::string_view tag_, const Position& position_);

        const GameObject* game_object;
        const std::string_view tag;
        Position position;
    };

    CollisionSystem() = default;

    bool IsRegistered(const GameObject* game_object) const;
    const Collider* GetRegisteredColliderOf(const GameObject* game_object) const;

    void RegisterColliderOf(const GameObject* game_object, const Collider* collider);
    void UnregisterColliderOf(const GameObject* game_object);

    std::optional<Position> CheckCollision(const GameObject* first, const GameObject* second) const;
    std::vector<CollisionInfo> GetAllCollisions(const GameObject* game_object) const;

private:
    std::unordered_map<const GameObject*, const Collider*> registered_colliders_;
};
