#include "Engine.h"

Engine& Engine::GetInstance() {
    static Engine instance;
    return instance;
}

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
            "The the pointed object was not created using "
            "the engine or was already destroyed");
}

Position Engine::GetCameraPosition() const {
    render_.GetCameraPosition();
}

void Engine::SetCameraOn(const GameObject *object) {
    render_.SetCameraOn(object);
}

void Engine::RenderAll() const {
    render_.RenderAll();
}

CollisionSystem::CollisionsInfoArray Engine::GetAllCollisions(const GameObject *game_object) const {
    return collision_system_.GetAllCollisions(game_object);
}

CollisionSystem::PossiblePosition Engine::CheckCollision(const GameObject *first, const GameObject *second) const {
    return collision_system_.CheckCollision(first, second);
}

Engine::~Engine() {
    for (const auto &object_ptr : objects_buffer_) {
        delete object_ptr;
    }

}

Engine::Engine(): collision_system_(CollisionSystem::GetInstance()), input_system_(InputSystem::GetInstance()), delay_queue_(DelayQueue::GetInstance()), ticks_count_(0) {}

void Engine::ReadNewInput() {
    input_system_.ReadNewInput();
}

InputSystem::InputTokensArray Engine::GetInput() const {
    return input_system_.GetInput();
}

void Engine::TryExecuteDelayedCallbacks() {
    delay_queue_.TryExecute(std::chrono::steady_clock::now(), ticks_count_);
}
