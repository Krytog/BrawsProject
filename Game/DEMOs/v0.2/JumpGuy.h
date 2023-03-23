#pragma once

#pragma once

#include "../../../Game/Core/Engine.h"
#include <unordered_set>

#include <iostream>

class Jumper : public GameObject {
public:
    Jumper(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& col_ptr, std::unique_ptr<VisibleObject>& vis_ptr, const std::string_view& tag):
            GameObject(pos_ptr, col_ptr, vis_ptr, tag),
            engine_(&Engine::GetInstance()) {

        const Vector2D& direction = Vector2D(3, 4);
        engine_->Invoke(std::chrono::seconds(3), dynamic_cast<GameObject*>(this), &GameObject::Translate, static_cast<const Vector2D&>(direction));
        temp_->insert(this);
        std::cout << "Another bullet is created" << std::endl;
    }


    static double SPEED;

    void Fly() {
        Vector2D to_translate = dir_ * SPEED;
        position_->Translate(to_translate);
        visible_object_->Translate(to_translate);
    }

    ~Jumper() {
        temp_->erase(this);
        std::cout << "Another bullet is destroyed" << std::endl;
    }

private:

    Vector2D GetRandDirection() {
        return Vector2D(std::rand() % 50 - 25, std::rand() % 50 - 25);
    }

    Engine* engine_;
};
