#pragma once

#include "../../../Game/Core/Engine.h"
#include <unordered_set>

#include <iostream>

class Bullet : public GameObject {
public:
    Bullet(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& col_ptr, std::unique_ptr<VisibleObject>& vis_ptr, const std::string_view& tag, const Vector2D& dir, std::unordered_set<Bullet*>* temp):
    GameObject(pos_ptr, col_ptr, vis_ptr, tag),
    engine_(&Engine::GetInstance()), temp_(temp), dir_(dir) {
        engine_->Invoke(std::chrono::seconds(3), engine_, &Engine::Destroy, dynamic_cast<GameObject*>(this));
        temp_->insert(this);
        std::cout << "Another bullet is created" << std::endl;
    }


    static double SPEED;

    void Fly() {
        Vector2D to_translate = dir_ * SPEED;
        position_->Translate(to_translate);
        visible_object_->Translate(to_translate);
    }

    ~Bullet() {
        temp_->erase(this);
        std::cout << "Another bullet is destroyed" << std::endl;
    }

private:
    Engine* engine_;
    Vector2D dir_;
    std::unordered_set<Bullet*>* temp_;
};

