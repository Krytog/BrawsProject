#include "CollisionSystem.h"

CollisionSystem::CollisionInfo::CollisionInfo(const GameObject* game_object_, const std::string_view tag_,
                                              const Position& position_, const bool is_trigger_)
    : game_object(game_object_), tag(tag_), position(position_), is_trigger(is_trigger_) {
}

bool CollisionSystem::IsRegistered(const GameObject* game_object) const {
    return registered_colliders_.contains(game_object);
}

const Collider* CollisionSystem::GetRegisteredColliderOf(const GameObject* game_object) const {
    return registered_colliders_.at(game_object);
}

void CollisionSystem::UnregisterColliderOf(const GameObject* game_object) {
    if (registered_colliders_.contains(game_object)) {
        registered_colliders_.erase(game_object);
    }
}

void CollisionSystem::RegisterColliderOf(const GameObject* game_object, const Collider* collider) {
    registered_colliders_[game_object] = collider;
}

CollisionSystem::PossiblePosition CollisionSystem::CheckPhysicalCollision(const GameObject* first,
                                                                          const GameObject* second) const {
    if (!registered_colliders_.contains(first) || !registered_colliders_.contains(second)) {
        return std::nullopt;
    }

    if (registered_colliders_.at(second).IsTrigger()) {
        return std::nullopt;
    }

    return registered_colliders_.at(first)->GetIntersectionPosition(registered_colliders_.at(second));
}

CollisionSystem::PossiblePosition CollisionSystem::CheckTriggerCollision(const GameObject* first,
                                                                         const GameObject* second) const {
    if (!registered_colliders_.contains(first) || !registered_colliders_.contains(second)) {
        return std::nullopt;
    }

    if (!registered_colliders_.at(second).IsTrigger()) {
        return std::nullopt;
    }

    return registered_colliders_.at(first)->GetIntersectionPosition(registered_colliders_.at(second));
}

CollisionSystem::CollisionsInfoArray CollisionSystem::GetAllCollisions(const GameObject* game_object) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& pair : registered_colliders_) {
        if (pair.first == game_object) {
            continue;
        }
        if (auto position = registered_colliders_.at(game_object)->GetIntersectionPosition(pair.second)) {
            output.emplace_back(pair.first, pair.first->GetTag(), position.value(), pair.second->IsTrigger());
        }
    }
    return output;
}

CollisionSystem::CollisionsInfoArray CollisionSystem::GetPhysicalCollisions(
    const GameObject* game_object) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& pair : registered_colliders_) {
        if (pair.second->IsTrigger()) {
            continue;
        }
        if (pair.first == game_object) {
            continue;
        }
        if (auto position = registered_colliders_.at(game_object)->GetIntersectionPosition(pair.second)) {
            output.emplace_back(pair.first, pair.first->GetTag(), position.value(), pair.second->IsTrigger());
        }
    }
    return output;
}

CollisionSystem::CollisionsInfoArray CollisionSystem::GetTriggerCollisions(
    const GameObject* game_object) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& pair : registered_colliders_) {
        if (!pair.second->IsTrigger()) {
            continue;
        }
        if (pair.first == game_object) {
            continue;
        }
        if (auto position = registered_colliders_.at(game_object)->GetIntersectionPosition(pair.second)) {
            output.emplace_back(pair.first, pair.first->GetTag(), position.value(), pair.second->IsTrigger());
        }
    }
    return output;
}

CollisionSystem::CollisionsInfoArray CollisionSystem::GetAllCollisionsWithTag(
    const GameObject* game_object, const std::string_view string) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& pair : registered_colliders_) {
        if (pair.first->GetTag() != string) {
            continue;
        }
        if (pair.first == game_object) {
            continue;
        }
        if (auto position = registered_colliders_.at(game_object)->GetIntersectionPosition(pair.second)) {
            output.emplace_back(pair.first, pair.first->GetTag(), position.value(), pair.second->IsTrigger());
        }
    }
    return output;
}

template <typename T>
CollisionSystem::CollisionsInfoArray CollisionSystem::GetAllCollisionsWithType(
    const GameObject* game_object) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& pair : registered_colliders_) {
        if (!dynamic_cast<T*>(pair.first)) {
            continue;
        }
        if (pair.first == game_object) {
            continue;
        }
        if (auto position = registered_colliders_.at(game_object)->GetIntersectionPosition(pair.second)) {
            output.emplace_back(pair.first, pair.first->GetTag(), position.value(), pair.second->IsTrigger());
        }
    }
    return output;
}

CollisionSystem& CollisionSystem::GetInstance() {
    static CollisionSystem instance;
    return instance;
}

CollisionSystem::CollisionSystem() = default;
