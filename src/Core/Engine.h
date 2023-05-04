#pragma once

#include <initializer_list>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "CollisionSystem.h"
#include "Colliders.h"
#include "GameObject.h"
#include "DelayQueue.h"
#include "EventHandler.h"

#ifndef __SERVER_ENGINE_MODE__
#include "Render.h"
#include "Sprites.h"
#include "InputSystem.h"
#endif

/* Depending on the compile options - __SERVER_ENGINE_MODE__ - the functionality may vary */
class Engine {
public:
    static Engine& GetInstance();

    // Creating and destroying objects
    ///////////////////////////////////////////////////////////////////////////////////////
    template <class TObject, typename... Args>
    GameObject* CreateGameObject(Args&&... args) {
        static_assert(std::is_base_of<GameObject, TObject>(), "TObject must inherit from GameObject");

        GameObject* object_ptr = new TObject(std::forward<Args>(args)...);
        objects_.push_front(object_ptr);
        cache_[object_ptr] = objects_.begin();

        auto coll_ptr = object_ptr->GetPointerToCollider();
        if (coll_ptr) {
            collision_system_.RegisterColliderOf(object_ptr, coll_ptr);
        }

#ifndef __SERVER_ENGINE_MODE__
        auto vis_ptr = object_ptr->GetPointerToVisibleObject();
        if (vis_ptr) {
            render_.AddToRender(object_ptr, vis_ptr);
        }
#endif
        return object_ptr;
    }

    template <class TObject>
    GameObject* CreateGameObjectByDefault() {
        static_assert(std::is_base_of<GameObject, TObject>(), "TObject must inherit from GameObject");

        GameObject* object_ptr = new TObject();
        objects_.push_front(object_ptr);
        cache_[object_ptr] = objects_.begin();

        auto coll_ptr = object_ptr->GetPointerToCollider();
        if (coll_ptr) {
            collision_system_.RegisterColliderOf(object_ptr, coll_ptr);
        }
#ifndef __SERVER_ENGINE_MODE__
        auto vis_ptr = object_ptr->GetPointerToVisibleObject();
        if (vis_ptr) {
            render_.AddToRender(object_ptr, vis_ptr);
        }
#endif
        return object_ptr;
    }

    void Destroy(GameObject* object_ptr);
    ///////////////////////////////////////////////////////////////////////////////////////

#ifndef __SERVER_ENGINE_MODE__

    // Render
    void RenderSwitch(GameObject* game_object, VisibleObject* new_visible_object);

    // Camera controls
    ///////////////////////////////////////////////////////////////////////////////////////
    void SetCameraOn(const GameObject* object);
    Position GetCameraPosition() const;
    ///////////////////////////////////////////////////////////////////////////////////////

    // Getting input
    ///////////////////////////////////////////////////////////////////////////////////////
    using DefaultInputSystem = KeyboardInputSystem;
    InputSystem::InputTokensArray GetInput() const;

    // Switch InputSystem
    template <InputSystemDerived SomeInputSystem>
    void SwitchInputSystem() {
        input_system_ = InputSystem::GetInstance<SomeInputSystem>(*render_.GetWindowPointer());
    }
    ///////////////////////////////////////////////////////////////////////////////////////

#endif

    // Collisions
    ///////////////////////////////////////////////////////////////////////////////////////
    CollisionSystem::PossiblePosition CheckPhysicalCollision(GameObject* first,
                                                             GameObject* second) const;
    CollisionSystem::PossiblePosition CheckTriggerCollision(GameObject* first,
                                                            GameObject* second) const;

    CollisionSystem::CollisionsInfoArray GetAllCollisions(GameObject* game_object) const;
    CollisionSystem::CollisionsInfoArray GetPhysicalCollisions(GameObject* game_object) const;
    CollisionSystem::CollisionsInfoArray GetTriggerCollisions(GameObject* game_object) const;
    CollisionSystem::CollisionsInfoArray GetAllCollisionsWithTag(GameObject* game_object,
                                                                 const std::string_view string) const;
    template <typename T>
    CollisionSystem::CollisionsInfoArray GetAllCollisionsWithType(GameObject* game_object) const;
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
    uint64_t GetTicksCount() const;
    ///////////////////////////////////////////////////////////////////////////////////////

    ~Engine();

    // This function should be called exactly once per game tick
    void Update();

    // The next functions can be used only if Update() is not called
    // The following order must be maintained
    ///////////////////////////////////////////////////////////////////////////////////////
#ifndef __SERVER_ENGINE_MODE__
    void ReadNewInput();
#endif
    void ExecuteUpdates();
    void TryExecuteDelayedCallbacks();
    void TryExecuteEvents();
    void IncreaseTicksCount();
#ifndef __SERVER_ENGINE_MODE__
    void RenderAll();
#endif
    ///////////////////////////////////////////////////////////////////////////////////////

    // Status of engine
    ///////////////////////////////////////////////////////////////////////////////////////
    bool IsActive() const;
    void SetActiveOn();
    void SetActiveOff();

private:
    Engine();
    void ClearAll();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

#ifndef __SERVER_ENGINE_MODE__
    Render& render_;
    InputSystem* input_system_;
#endif
    CollisionSystem& collision_system_;
    EventHandler& event_handler_;
    DelayQueue& delay_queue_;

    /// \brief Data structure to manage objects
    std::list<GameObject*> objects_;
    std::unordered_map<GameObject*, std::list<GameObject*>::iterator> cache_;

    uint64_t ticks_count_ = 0;
    bool is_active_ = true;
};