#include "Engine.h"

template <class TObject, typename... Args>
GameObject *Engine::ProduceObject(Args&&... args, Position *pos_ptr, Collider *coll_ptr,
                                  VisibleObject *vis_ptr, const std::string_view &tag) {
    static_assert(std::is_base_of<GameObject, TObject>(), "TObject must inherit from GameObject");

    GameObject *object_ptr =
            new TObject(std::forward<Args>(args)..., std::unique_ptr<Position>(pos_ptr), std::unique_ptr<Collider>(coll_ptr),
                        std::unique_ptr<VisibleObject>(vis_ptr), tag);
    objects_buffer_.push_back(object_ptr);
    if (coll_ptr) {
        collision_system_.RegisterColliderOf(object_ptr, coll_ptr);
    }
    if (vis_ptr) {
        render_.AddToRender(object_ptr, vis_ptr);
    }
    return object_ptr;
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
            "The object under the pointer was not created using "
            "the engine or was destroyed");
}

Engine::~Engine() {
    for (const auto &object_ptr : objects_buffer_) {
        delete object_ptr;
    }
}

Engine &Engine::getInstance() {
    static Engine instance;
    return instance;
}

Engine::Engine() = default;
