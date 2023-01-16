#include "Engine.h"

void Engine::Destroy(GameObject *object_ptr) {
    auto it = std::find(objects_buffer_.begin(), objects_buffer_.end(), object_ptr);
    if (it != objects_buffer_.end()) {
        objects_buffer_.erase(it);
        collision_system_.UnregisterColliderOf(object_ptr);
        render_.RemoveFromRender(object_ptr);
        delete object_ptr;
        return;
    }

    throw std::runtime_error(
        "The object under the pointer was not created using "
        "the engine or was destroyed");
}

Position Engine::GetCameraPosition() const {
    render_.GetCameraPosition();
}

void Engine::SetCameraOn(const GameObject *object) {
    render_.SetCameraOn(object);
}

//Временно
void Engine::RenderAll() {
    render_.RenderAll();
}

std::vector<CollisionSystem::CollisionInfo> Engine::GetAllCollisions(GameObject *game_object) {
    return collision_system_.GetAllCollisions(game_object);
}

Engine::~Engine() {
    for (const auto &object_ptr : objects_buffer_) {
        delete object_ptr;
    }
}