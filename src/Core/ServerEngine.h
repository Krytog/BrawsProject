#pragma once

#include "Engine.h"
#include "GameObject.h"

template <>
class Engine<GameObject, __EngineCreation::Initialization> final
    : public Engine<GameObject, __EngineCreation::Skeleton> {
public:
    static Engine& GetInstance() {
        static Engine instance;
        return instance;
    }

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
        return object_ptr;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void Destroy(GameObject* object_ptr) {
        if (cache_.find(object_ptr) != cache_.end()) {
            objects_.erase(cache_[object_ptr]);
            cache_.erase(object_ptr);
            collision_system_.UnregisterColliderOf(object_ptr);
            delete object_ptr;
            return;
        }
        throw std::runtime_error(
            "The pointed object was not created using "
            "the engine or was already destroyed");
    }

    // This function should be called exactly once per game tick
    void Update() {
        ExecuteUpdates();
        TryExecuteDelayedCallbacks();
        TryExecuteEvents();
        IncreaseTicksCount();
    }
};

// Engine that works with GameObjects
using ServerEngine = Engine<GameObject, __EngineCreation::Initialization>;
