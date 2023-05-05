#include "Engine.h"

Engine& Engine::GetInstance() {
    static Engine instance;
    return instance;
}

void Engine::Destroy(GameObject* object_ptr) {
    if (cache_.find(object_ptr) != cache_.end()) {
        objects_.erase(cache_[object_ptr]);
        cache_.erase(object_ptr);
        collision_system_.UnregisterColliderOf(object_ptr);
#ifndef __SERVER_ENGINE_MODE__
        render_.RemoveFromRender(object_ptr);
#endif
        delete object_ptr;
        return;
    }

    throw std::runtime_error(
        "The pointed object was not created using "
        "the engine or was already destroyed");
}

#ifndef __SERVER_ENGINE_MODE__
Position Engine::GetCameraPosition() const {
    return render_.GetCameraPosition();
}

void Engine::SetCameraOn(const GameObject* object) {
    render_.SetCameraOn(object);
}

void Engine::RenderAll() {
    render_.RenderAll();
}
#endif

void Engine::ClearAll() {
    for (const auto& object_ptr : objects_) {
        delete object_ptr;
    }
    cache_.clear();
}

Engine::~Engine() {
    ClearAll();
}

Engine::Engine()
    : collision_system_(CollisionSystem::GetInstance()),
#ifndef __SERVER_ENGINE_MODE__
      render_(Render::GetInstance()),
      input_system_(InputSystem::GetInstance<DefaultInputSystem>(*render_.GetWindowPointer())),
#endif
      event_handler_(EventHandler::GetInstance()),
      delay_queue_(DelayQueue::GetInstance()),
      ticks_count_(0) {
}

#ifndef __SERVER_ENGINE_MODE__
void Engine::ReadNewInput() {
    input_system_->ReadNewInput();
}

InputSystem::InputTokensArray Engine::GetInput() const {
    auto raw_input = input_system_->GetInput();
    auto mouse_token_variant = *(raw_input.begin());
    auto mouse_token = std::get<InputSystem::MouseToken>(mouse_token_variant);  //Make sure that MouseToken is the first token
    auto window_size = render_.GetWindowPointer()->getSize();
    Vector2D difference = Vector2D(static_cast<double>(window_size.x) / 2, static_cast<double>(window_size.y) / 2) - mouse_token.position.GetCoordinatesAsVector2D();
    Position global_mouse_pos(render_.GetCameraPosition().GetCoordinatesAsVector2D() - difference);
    *(raw_input.begin()) = InputSystem::MouseToken{mouse_token.key, global_mouse_pos};
    return raw_input;
}
#endif

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

void Engine::ExecuteUpdates() {
    for (auto game_object : objects_) {
        game_object->OnUpdate();
    }
}

void Engine::TryExecuteEvents() {
    event_handler_.TryExecuteAll();
}

void Engine::Update() {
#ifndef __SERVER_ENGINE_MODE__
    ReadNewInput();
#endif
    ExecuteUpdates();
    TryExecuteDelayedCallbacks();
    TryExecuteEvents();
#ifndef __SERVER_ENGINE_MODE__
    RenderAll();
#endif
    IncreaseTicksCount();
}

#ifndef __SERVER_ENGINE_MODE__
void Engine::RenderSwitch(GameObject *game_object, VisibleObject *new_visible_object) {
    render_.RemoveFromRender(game_object);
    render_.AddToRender(game_object, new_visible_object);
}
#endif

bool Engine::IsActive() const {
    return is_active_;
}

void Engine::SetActiveOn() {
    is_active_ = true;
}

void Engine::SetActiveOff() {
    is_active_ = false;
}
