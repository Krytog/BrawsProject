#include "CustomBehaviour.h"

Engine& Engine::GetInstance() {
    static Engine instance;
    return instance;
}

void Engine::Destroy(GameObject* object_ptr) {
    auto it = std::find(objects_buffer_.begin(), objects_buffer_.end(), object_ptr);
    if (it != objects_buffer_.end()) {
        objects_buffer_.erase(it);
        collision_system_.UnregisterColliderOf(object_ptr);
        render_.RemoveFromRender(object_ptr);
        delete object_ptr;
        return;
    }

    throw std::runtime_error(
        "The pointed object was not created using "
        "the engine or was already destroyed");
}

Position Engine::GetCameraPosition() const {
    return render_.GetCameraPosition();
}

void Engine::SetCameraOn(const GameObject* object) {
    render_.SetCameraOn(object);
}

void Engine::RenderAll() {
    render_.RenderAll();
}

Engine::~Engine() {
    for (const auto& object_ptr : objects_buffer_) {
        delete object_ptr;
    }

}

Engine::Engine()
    : collision_system_(CollisionSystem::GetInstance()),
      input_system_(InputSystem::GetInstance(*render_.GetWindowPointer())),
      event_handler_(EventHandler::GetInstance()),
      delay_queue_(DelayQueue::GetInstance()),
      ticks_count_(0) {
}

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

CollisionSystem::CollisionsInfoArray Engine::GetAllCollisions(GameObject* game_object) const {
    return collision_system_.GetAllCollisions(game_object);
}

CollisionSystem::CollisionsInfoArray Engine::GetPhysicalCollisions(GameObject* game_object) const {
    return collision_system_.GetPhysicalCollisions(game_object);
}

CollisionSystem::CollisionsInfoArray Engine::GetTriggerCollisions(GameObject* game_object) const {
    return collision_system_.GetTriggerCollisions(game_object);
}

CollisionSystem::CollisionsInfoArray Engine::GetAllCollisionsWithTag(GameObject* game_object,
                                                                     const std::string_view string) const {
    return collision_system_.GetAllCollisionsWithTag(game_object, string);
}

template <typename T>
CollisionSystem::CollisionsInfoArray Engine::GetAllCollisionsWithType(GameObject* game_object) const {
    return collision_system_.GetAllCollisionsWithType<T>(game_object);
}

CollisionSystem::PossiblePosition Engine::CheckPhysicalCollision(GameObject* first,
                                                                 GameObject* second) const {
    return collision_system_.CheckPhysicalCollision(first, second);
}

CollisionSystem::PossiblePosition Engine::CheckTriggerCollision(GameObject* first,
                                                                GameObject* second) const {
    return collision_system_.CheckTriggerCollision(first, second);
}

uint64_t Engine::GetTicksCount() const {
    return ticks_count_;
}

void Engine::IncreaseTicksCount() {
    ++ticks_count_;
}

void Engine::ExecuteUpdatesOfCustomBehaviours() {
    for (auto game_object : objects_buffer_) {
        if (auto custom = dynamic_cast<CustomBehaviour*>(game_object)) {
            custom->OnUpdate();
        }
    }
}

void Engine::TryExecuteEvents() {
    event_handler_.TryExecuteAll();
}

void Engine::Update() {
    ReadNewInput();
    ExecuteUpdatesOfCustomBehaviours();
    TryExecuteDelayedCallbacks();
    TryExecuteEvents();
    RenderAll();
    IncreaseTicksCount();
}

void Engine::RenderSwith(GameObject *game_object, VisibleObject *new_visible_object) {
    render_.RemoveFromRender(game_object);
    render_.AddToRender(game_object, new_visible_object);
}