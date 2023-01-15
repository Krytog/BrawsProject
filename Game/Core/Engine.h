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
    GameObject *ProduceObject(Args &&... args, Position *pos_ptr, Collider *coll_ptr,
                              VisibleObject *vis_ptr, const std::string_view &tag);

    void Destroy(GameObject *object_ptr);

    ~Engine();

private:
    Engine();

    CollisionSystem collision_system_;
    Render render_;
    std::vector<GameObject *> objects_buffer_;
};