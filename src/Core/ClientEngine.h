#pragma once

#include "Engine.h"
#include "VisibleGameObject.h"
#include "Tools/Concepts.h"

#include "Qt/MainWidget/Render.h"
#include "Qt/PaintTemplates/BasicSprite.h"
#include "Qt/PaintTemplates/AnimatedSprite.h"
#include "InputSystem.h"

#include "../BotAPI/BotInputSystem.h"
#include "../BotAPI/BotManagement.h"

template <>
class Engine<VisibleGameObject, __EngineCreation::Initialization> final
    : public Engine<VisibleGameObject, __EngineCreation::Skeleton> {
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

        VisibleGameObject* object_ptr = new TObject(std::forward<Args>(args)...);
        objects_.push_front(object_ptr);
        cache_[object_ptr] = objects_.begin();

        auto coll_ptr = object_ptr->GetPointerToCollider();
        if (coll_ptr) {
            collision_system_.RegisterColliderOf(object_ptr, coll_ptr);
        }

        auto vis_ptr = object_ptr->GetPointerToVisibleObject();
        if (vis_ptr) {
            render_.AddToRender(object_ptr, vis_ptr);
        }
        return object_ptr;
    }

    template <class TObject>
    GameObject* CreateGameObjectByDefault() {
        static_assert(std::is_base_of<GameObject, TObject>(), "TObject must inherit from GameObject");

        VisibleGameObject* object_ptr = new TObject();
        objects_.push_front(object_ptr);
        cache_[object_ptr] = objects_.begin();

        auto coll_ptr = object_ptr->GetPointerToCollider();
        if (coll_ptr) {
            collision_system_.RegisterColliderOf(object_ptr, coll_ptr);
        }
        auto vis_ptr = object_ptr->GetPointerToVisibleObject();
        if (vis_ptr) {
            render_.AddToRender(object_ptr, vis_ptr);
        }
        return object_ptr;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void Destroy(GameObject* object_ptr) {
        if (cache_.find(object_ptr) != cache_.end()) {
            objects_.erase(cache_[object_ptr]);
            cache_.erase(object_ptr);
            collision_system_.UnregisterColliderOf(object_ptr);
            render_.RemoveFromRender(object_ptr);
            delete object_ptr;
            return;
        }
        throw std::runtime_error(
            "The pointed object was not created using "
            "the engine or was already destroyed");
    }

    // Camera controls
    ///////////////////////////////////////////////////////////////////////////////////////
    void SetCameraOn(GameObject* object) {
        BotManagement::GetInstance().ObserveBot(object);
        render_.SetCameraOn(object);
    }
    Position GetCameraPosition() const {
        return render_.GetCameraPosition();
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    // Getting input
    ///////////////////////////////////////////////////////////////////////////////////////
    using DefaultInputSystem = KeyboardInputSystem;
    InputSystem::InputTokensArray GetInput() const {
        return input_system_->GetInput();
    }

    // Switch InputSystem
    template <InputSystemDerived SomeInputSystem>
    void SwitchInputSystem() {
        input_system_ = InputSystem::GetInstance<SomeInputSystem>();
    }
    ///////////////////////////////////////////////////////////////////////////////////////

    // Additional functions to handle Input and VisibleObjects in Update()
    void ReadNewInput() {
        input_system_->ReadNewInput();
    }
    void RenderAll() {
        render_.RenderAll();
    }

    // This function should be called exactly once per game tick
    void Update() {
        ReadNewInput();
        ExecuteUpdates();
        TryExecuteDelayedCallbacks();
        TryExecuteEvents();
        RenderAll();
        IncreaseTicksCount();
    }

private:
    Engine() : Engine<VisibleGameObject, __EngineCreation::Skeleton>(),
          render_(Render::GetInstance()),
          input_system_(InputSystem::GetInstance<DefaultInputSystem>()){
    }
    Render& render_;
    InputSystem* input_system_;
};

// Engine that works with VisibleObjects
using ClientEngine = Engine<VisibleGameObject, __EngineCreation::Initialization>;