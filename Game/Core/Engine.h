#pragma once

#include <initializer_list>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "CollisionSystem.h"
#include "Colliders.h"
#include "GameObject.h"
#include "Render.h"
#include "Sprites.h"
#include "DelayQueue.h"
#include "InputSystem.h"

class Engine {
public:
    static Engine& GetInstance();

    template <class TObject, typename... Args>
    GameObject* ProduceObject(Position* pos_ptr, Collider* coll_ptr, VisibleObject* vis_ptr,
                              const std::string_view& tag, Args&&... args) {
        static_assert(std::is_base_of<GameObject, TObject>(), "TObject must inherit from GameObject");

        std::unique_ptr<Position> pos_uptr(pos_ptr);
        std::unique_ptr<Collider> coll_uptr(coll_ptr);
        std::unique_ptr<VisibleObject> vis_uptr(vis_ptr);
        GameObject* object_ptr = new TObject(pos_uptr, coll_uptr, vis_uptr, tag, std::forward<Args>(args)...);
        objects_buffer_.push_back(object_ptr);
        if (coll_ptr) {
            collision_system_.RegisterColliderOf(object_ptr, coll_ptr);
        }
        if (vis_ptr) {
            render_.AddToRender(object_ptr, vis_ptr);
        }
        return object_ptr;
    }

    void Destroy(GameObject* object_ptr);

    void SetCameraOn(const GameObject* object);

    Position GetCameraPosition() const;

    // temporary public, should be private in the future as it will be called in Update method of
    // Engine;
    void RenderAll() const;

    CollisionSystem::PossiblePosition CheckPhysicalCollision(const GameObject* first,
                                                             const GameObject* second) const;
    CollisionSystem::PossiblePosition CheckTriggerCollision(const GameObject* first,
                                                            const GameObject* second) const;

    CollisionSystem::CollisionsInfoArray GetAllCollisions(const GameObject* game_object) const;
    CollisionSystem::CollisionsInfoArray GetPhysicalCollisions(const GameObject* game_object) const;
    CollisionSystem::CollisionsInfoArray GetTriggerCollisions(const GameObject* game_object) const;
    CollisionSystem::CollisionsInfoArray GetAllCollisionsWithTag(const GameObject* game_object,
                                                                 const std::string_view string) const;
    template <typename T>
    CollisionSystem::CollisionsInfoArray GetAllCollisionsWithType(const GameObject* game_object) const;

    void ReadNewInput();

    InputSystem::InputTokensArray GetInput() const;

    template <typename Callable, typename... Args>
    void Invoke(const std::chrono::milliseconds& milliseconds, Callable&& cb, Args... args) {
        delay_queue_.PushTime(std::chrono::steady_clock::now() + milliseconds, std::forward<Callable>(cb),
                              std::forward<Args>(args)...);
    }
    template <typename Callable, typename... Args>
    void Invoke(const uint64_t ticks_count, Callable&& cb, Args... args) {
        delay_queue_.PushTicks(ticks_count, std::forward<Callable>(cb), std::forward<Args>(args)...);
    }
    template <typename F, typename Callable, typename... Args>
    void Invoke(const std::chrono::milliseconds& milliseconds, F* pointer, Callable&& cb, Args... args) {
        delay_queue_.PushTime(std::chrono::steady_clock::now() + milliseconds, pointer,
                              std::forward<Callable>(cb), std::forward<Args>(args)...);
    }
    template <typename F, typename Callable, typename... Args>
    void Invoke(const uint64_t ticks_count, F* pointer, Callable&& cb, Args... args) {
        delay_queue_.PushTicks(ticks_count, pointer, std::forward<Callable>(cb), std::forward<Args>(args)...);
    }

    // temporary public, should be private in the future as it will be called in Update method of
    // Engine;
    void TryExecuteDelayedCallbacks();

    ~Engine();

private:
    Engine();

    CollisionSystem collision_system_;
    mutable Render render_;
    InputSystem input_system_;
    DelayQueue delay_queue_;

    std::vector<GameObject*> objects_buffer_;

public:
    uint64_t ticks_count_ = 0;
};
