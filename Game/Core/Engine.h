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

class Engine {
public:
    static Engine &getInstance() {
        static Engine instance;
        return instance;
    }

    template <class TObject, typename... Args>
    GameObject *ProduceObject(Position *pos_ptr, Collider *coll_ptr, VisibleObject *vis_ptr,
                              const std::string_view &tag, Args &&... args) {
        static_assert(std::is_base_of<GameObject, TObject>(),
                      "TObject must inherit from GameObject");

        std::unique_ptr<Position> pos_uptr(pos_ptr);
        std::unique_ptr<Collider> coll_uptr(coll_ptr);
        std::unique_ptr<VisibleObject> vis_uptr(vis_ptr);
        GameObject *object_ptr =
            new TObject(pos_uptr, coll_uptr, vis_uptr, tag, std::forward<Args>(args)...);
        objects_buffer_.push_back(object_ptr);
        collision_system_.RegisterColliderOf(object_ptr, coll_ptr);
        render_.AddToRender(object_ptr, vis_ptr);
        return object_ptr;
    }

    void Destroy(GameObject *object_ptr);

    void SetCameraOn(const GameObject *object);
    Position GetCameraPosition() const;

    //Временно
    void RenderAll();
    std::vector<CollisionSystem::CollisionInfo> GetAllCollisions(GameObject *game_object);
    ~Engine();

private:
    Engine() = default;

    CollisionSystem collision_system_;
    Render render_;
    std::vector<GameObject *> objects_buffer_;
};