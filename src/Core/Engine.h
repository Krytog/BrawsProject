#pragma once

#include <initializer_list>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <cstdint>

#define FIELD_OF_VIEW_WIDTH 1280
#define FIELD_OF_VIEW_HEIGHT 720

#include "CollisionSystem.h"
#include "Colliders.h"
#include "DelayQueue.h"
#include "EventHandler.h"

class GameObject;         // for server engine  ~-~  without Visible part
class VisibleGameObject;  // for client engine  ~-~  with Visible part

/* Never to be used */
enum __EngineCreation : uint8_t /* Hide */ {
    Skeleton = 0,
    Initialization = 1,
};

template <typename Object>
concept EngineObject = std::is_same_v<Object, GameObject> || std::is_same_v<Object, VisibleGameObject>;

template <EngineObject T, __EngineCreation status>
class Engine;

template <EngineObject T>
class Engine<T, __EngineCreation::Skeleton> {
public:
    void DestroyEvent(Event* event) {
        event_handler_.DestroyEvent(event);
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    // Collisions
    ///////////////////////////////////////////////////////////////////////////////////////
    CollisionSystem::PossiblePosition CheckPhysicalCollision(GameObject* first, GameObject* second) const {
        return collision_system_.CheckPhysicalCollision(first, second);
    }
    CollisionSystem::PossiblePosition CheckTriggerCollision(GameObject* first, GameObject* second) const {
        return collision_system_.CheckTriggerCollision(first, second);
    }

    CollisionSystem::CollisionsInfoArray GetAllCollisions(GameObject* game_object) const {
        return collision_system_.GetAllCollisions(game_object);
    }
    CollisionSystem::CollisionsInfoArray GetPhysicalCollisions(GameObject* game_object) const {
        return collision_system_.GetPhysicalCollisions(game_object);
    }
    CollisionSystem::CollisionsInfoArray GetTriggerCollisions(GameObject* game_object) const {
        return collision_system_.GetTriggerCollisions(game_object);
    }
    CollisionSystem::CollisionsInfoArray GetAllCollisionsWithTag(GameObject* game_object,
                                                                 std::string_view string) const {
        return collision_system_.GetAllCollisionsWithTag(game_object, string);
    }
    template <typename U>
    CollisionSystem::CollisionsInfoArray GetAllCollisionsWithType(GameObject* game_object) const {
        return collision_system_.GetAllCollisionsWithType<U>(game_object);
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // Delayed callbacks
    ///////////////////////////////////////////////////////////////////////////////////////
    template <typename Callable, typename... Args>
    void Invoke(const std::chrono::milliseconds& milliseconds, Callable&& cb, Args&&... args) {
        delay_queue_.PushTime(std::chrono::steady_clock::now() + milliseconds, std::forward<Callable>(cb),
                              std::forward<Args>(args)...);
    }
    template <typename Callable, typename... Args>
    void Invoke(const uint64_t ticks_count, Callable&& cb, Args&&... args) {
        delay_queue_.PushTicks(ticks_count, std::forward<Callable>(cb), std::forward<Args>(args)...);
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // Events
    ///////////////////////////////////////////////////////////////////////////////////////
    template <typename Predicate, typename... PredicateArgs, typename Callable, typename... CallableArgs,
              typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>>
    Event* CreateEvent(Predicate&& pr, PredicateArgsTuple&& pr_args, Callable&& cb,
                       CallableArgsTuple&& cb_args, EventStatus status) {
        return event_handler_.CreateEvent(
            std::forward<Predicate>(pr), std::forward<PredicateArgsTuple>(pr_args),
            std::forward<Callable>(cb), std::forward<CallableArgsTuple>(cb_args), status);
    }

    template <typename Predicate, typename... PredicateArgs, typename Invoker, typename Callable,
              typename... CallableArgs, typename PredicateArgsTuple = std::tuple<PredicateArgs...>,
              typename CallableArgsTuple = std::tuple<CallableArgs...>>
    Event* CreateEvent(Predicate&& pr, PredicateArgsTuple&& pr_args, Invoker* pointer, Callable&& cb,
                       CallableArgsTuple&& cb_args, EventStatus status) {
        return event_handler_.CreateEvent(
            std::forward<Predicate>(pr), std::forward<PredicateArgsTuple>(pr_args), pointer,
            std::forward<Callable>(cb), std::forward<CallableArgsTuple>(cb_args), status);
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // Ticks
    ///////////////////////////////////////////////////////////////////////////////////////
    uint64_t GetTicksCount() const {
        return ticks_count_;
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    ~Engine() {
        ClearAll();
    }

    // The next functions can be used only if Update() is called
    // The following order must be maintained
    ///////////////////////////////////////////////////////////////////////////////////////
    void ExecuteUpdates() {
        for (const auto& object : objects_) {
            object->OnUpdate();
        }
    }
    void TryExecuteDelayedCallbacks() {
        delay_queue_.TryExecute(std::chrono::steady_clock::now(), ticks_count_);
    }
    void TryExecuteEvents() {
        event_handler_.TryExecuteAll();
    }
    void IncreaseTicksCount() {
        ++ticks_count_;
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    // Status of engine
    ///////////////////////////////////////////////////////////////////////////////////////
    bool IsActive() const {
        return is_active_;
    }
    void SetActiveOn() {
        is_active_ = true;
    }
    void SetActiveOff() {
        is_active_ = false;
    }

protected:
    Engine()
        : collision_system_(CollisionSystem::GetInstance()),
          event_handler_(EventHandler::GetInstance()),
          delay_queue_(DelayQueue::GetInstance()),
          ticks_count_(0) {
    }
    void ClearAll() {
        for (const auto& object_ptr : objects_) {
            delete object_ptr;
        }
        cache_.clear();
    }

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    CollisionSystem& collision_system_;
    EventHandler& event_handler_;
    DelayQueue& delay_queue_;

    // Data structure to manage objects
    std::list<GameObject*> objects_;
    FastHashMap<GameObject*, typename std::list<GameObject*>::iterator> cache_;

    uint64_t ticks_count_ = 0;
    bool is_active_ = true;
};
