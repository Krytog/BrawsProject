#include "CollisionSystem.h"

CollisionSystem::CollisionInfo::CollisionInfo(GameObject* game_object, const std::string_view tag,
                                              const Position& position, Collider::Category category)
    : game_object(game_object), tag(tag), position(position), category(category) {
}

bool CollisionSystem::IsRegistered(GameObject* game_object) const {
    return registered_colliders_.contains(game_object);
}

const Collider* CollisionSystem::GetRegisteredColliderOf(GameObject* game_object) const {
    return registered_colliders_.at(game_object);
}

void CollisionSystem::UnregisterColliderOf(GameObject* game_object) {
    if (registered_colliders_.contains(game_object)) {
        registered_colliders_.erase(game_object);
    }
}

void CollisionSystem::RegisterColliderOf(GameObject* game_object, const Collider* collider) {
    registered_colliders_[game_object] = collider;
}

CollisionSystem::PossiblePosition CollisionSystem::CheckPhysicalCollision(GameObject* first,
                                                                          GameObject* second) const {
    if (!registered_colliders_.contains(first) || !registered_colliders_.contains(second)) {
        return std::nullopt;
    }

    if (registered_colliders_.at(second)->IsTrigger()) {
        return std::nullopt;
    }

    return registered_colliders_.at(first)->GetIntersectionPosition(registered_colliders_.at(second));
}

CollisionSystem::PossiblePosition CollisionSystem::CheckTriggerCollision(GameObject* first,
                                                                         GameObject* second) const {
    if (!registered_colliders_.contains(first) || !registered_colliders_.contains(second)) {
        return std::nullopt;
    }

    if (!registered_colliders_.at(second)->IsTrigger()) {
        return std::nullopt;
    }

    return registered_colliders_.at(first)->GetIntersectionPosition(registered_colliders_.at(second));
}

CollisionSystem::CollisionsInfoArray CollisionSystem::GetAllCollisions(GameObject* game_object) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& [object, collider] : registered_colliders_) {
        if (object == game_object || collider->IsTechnical()) {
            continue;
        }
        if (auto position = registered_colliders_.at(game_object)->GetIntersectionPosition(collider)) {
            output.emplace_back(object, object->GetTag(), position.value(), collider->GetCategory());
        }
    }
    return output;
}

CollisionSystem::CollisionsInfoArray CollisionSystem::GetPhysicalCollisions(GameObject* game_object) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& [object, collider] : registered_colliders_) {
        if (!collider->IsOrdinary()) {  // not trigger && not technical
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

CollisionSystem::CollisionsInfoArray CollisionSystem::GetTriggerCollisions(GameObject* game_object) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& [object, collider] : registered_colliders_) {
        if (!collider->IsTrigger()) {  // not ordinary && not technical
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

CollisionSystem::CollisionsInfoArray CollisionSystem::GetAllCollisionsWithTag(
    GameObject* game_object, std::string_view string) const {
    if (!registered_colliders_.contains(game_object)) {
        return {};
    }
    std::vector<CollisionInfo> output;
    for (const auto& [object, collider] : registered_colliders_) {
        if (object->GetTag() != string || collider->IsTechnical()) {
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

template <typename T>
CollisionSystem::CollisionsInfoArray CollisionSystem::GetAllCollisionsWithType(
    GameObject* game_object) const {
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

CollisionSystem& CollisionSystem::GetInstance() {
    static CollisionSystem instance;
    return instance;
}

CollisionSystem::CollisionSystem() = default;
