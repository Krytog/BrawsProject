#include "CollisionSystem.h"

CollisionSystem::CollisionInfo::CollisionInfo(const GameObject *game_object_, const std::string_view tag_,
                                              const Position &position_): game_object(game_object_), tag(tag_), position(position_) {}

bool CollisionSystem::IsRegistered(const GameObject *game_object) const {
    return registered_colliders_.contains(game_object);
}

const Collider *CollisionSystem::GetRegisteredColliderOf(const GameObject *game_object) const {
    return registered_colliders_.at(game_object);
}

void CollisionSystem::UnregisterColliderOf(const GameObject *game_object) {
    if (registered_colliders_.contains(game_object)) {
        registered_colliders_.erase(game_object);
    }
}

void CollisionSystem::RegisterColliderOf(const GameObject *game_object, const Collider *collider) {
    registered_colliders_[game_object] = collider;
}

CollisionSystem::PossiblePosition CollisionSystem::CheckCollision(const GameObject *first, const GameObject *second) const {
    if (!registered_colliders_.contains(first) || !registered_colliders_.contains(second)) {
        return std::nullopt;
    }
    return registered_colliders_.at(first)->GetIntersectionPosition(registered_colliders_.at(second));
}

CollisionSystem::CollisionsInfoArray CollisionSystem::GetAllCollisions(const GameObject *game_object) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& pair : registered_colliders_) {
        if (pair.first == game_object) {
            continue;
        }
        if (auto position = registered_colliders_.at(game_object)->GetIntersectionPosition(pair.second)) {
            output.emplace_back(pair.first, pair.first->GetTag(), position.value());
        }
    }
    return output;
}

CollisionSystem &CollisionSystem::GetInstance() {
    static CollisionSystem instance;
    return instance;
}

CollisionSystem::CollisionSystem() = default;
