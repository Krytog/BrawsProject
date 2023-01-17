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
    return render_.GetCameraPosition();
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

Engine::Engine(): collision_system_(CollisionSystem::GetInstance()), render_(Render()), input_system_(InputSystem::GetInstance(*render_.GetWindowPointer())), delay_queue_(DelayQueue::GetInstance()), ticks_count_(0) {}

void Engine::ReadNewInput() {
    input_system_.ReadNewInput();
}

InputSystem::InputTokensArray Engine::GetInput() const {
    auto raw_input = input_system_.GetInput();
    auto mouse_token_variant = *(raw_input.begin());
    auto mouse_token = std::get<InputSystem::MouseToken>(mouse_token_variant);  //Make sure that MouseToken is the first token
    auto window_size = render_.GetWindowPointer()->getSize();
    Vector2D difference = Vector2D(static_cast<double>(window_size.x) / 2, static_cast<double>(window_size.y) / 2) - mouse_token.position.GetCoordinatesAsVector2D();
    Position global_mouse_pos(render_.GetCameraPosition().GetCoordinatesAsVector2D() - difference);
    *(raw_input.begin()) = InputSystem::MouseToken{mouse_token.key, global_mouse_pos};
    return raw_input;
}

void Engine::TryExecuteDelayedCallbacks() {
    delay_queue_.TryExecute(std::chrono::steady_clock::now(), ticks_count_);
}
