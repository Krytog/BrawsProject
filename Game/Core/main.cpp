#include "Sprites.h"
#include "GameObject.h"
#include "Render.h"
#include "MyTime.h"
#include "CollisionSystem.h"
#include "Colliders.h"
#include "Engine.h"

#include <iostream>

std::vector<Vector2D> CringeMovement() {
    std::vector<Vector2D> output;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        output.push_back(Vector2D::Right);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        output.push_back(Vector2D::Left);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        output.push_back(Vector2D::Up);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        output.push_back(Vector2D::Down);
    }
    return output;
}

int main() {
    MyTime time;

    Engine& engine = Engine::getInstance();

    auto pos_ptr = new Position(0, 0);
    auto game_obj = engine.ProduceObject<GameObject>(
        pos_ptr, new RectangleCollider(*pos_ptr, 200, 200),
        new StaticSprite(pos_ptr, 200, 200, std::string_view("../Game/Core/test_pics/kvaas.png")),
        std::string_view("kvas"));

    engine.SetCameraOn(game_obj);

    auto spider_pos = new Position(250, 250);
    auto spider_object = engine.ProduceObject<GameObject>(
        spider_pos, new RectangleCollider(*spider_pos, 250, 250),
        new StaticSprite(spider_pos, 250, 250,
                         std::string_view("../Game/Core/test_pics/download.jpeg")),
        std::string_view("wall"));

    //    Position pos2(482, 273);
    //    std::unique_ptr<VisibleObject> vis_ptr2 = std::make_unique<StaticSprite>(&pos2, 500, 500,
    //    std::string_view("../Game/Core/test_pics/download.jpeg")); auto bebra_vis2 =
    //    vis_ptr2.get(); auto pos_ptr2 = std::make_unique<Position>(pos2);
    //    std::unique_ptr<Collider> collider_ptr2 =
    //    std::make_unique<RectangleCollider>(Position(600, 380), 236, 214); auto bebra_col2 =
    //    collider_ptr2.get(); GameObject* game_obj2; game_obj2 = new GameObject(pos_ptr2,
    //    collider_ptr2, vis_ptr2, "wall"); render.AddToRender(game_obj2, bebra_vis2);
    //    collision_system.RegisterColliderOf(game_obj2, bebra_col2);
    //
    //
    //    Position pos3(1100, 660);
    //    std::unique_ptr<VisibleObject> vis_ptr3 = nullptr;
    //    auto bebra_vis3 = nullptr;
    //    auto pos_ptr3 = std::make_unique<Position>(pos3);
    //    std::unique_ptr<Collider> collider_ptr3 =
    //    std::make_unique<RectangleCollider>(Position(1100, 660), 200, 200, true); auto bebra_col3
    //    = collider_ptr3.get(); GameObject* game_obj3; game_obj3 = new GameObject(pos_ptr3,
    //    collider_ptr3, vis_ptr3, "win");
    //    //render.AddToRender(game_obj3, bebra_vis3);
    //    collision_system.RegisterColliderOf(game_obj3, bebra_col3);
    //
    //
    //    Position posw1(-5, 380);
    //    std::unique_ptr<VisibleObject> vis_ptrw1 = nullptr;
    //    auto bebra_visw1 = nullptr;
    //    auto pos_ptrw1 = std::make_unique<Position>(posw1);
    //    std::unique_ptr<Collider> collider_ptrw1 =
    //    std::make_unique<RectangleCollider>(Position(-5, 380), 10, 760, true); auto bebra_colw1 =
    //    collider_ptrw1.get(); GameObject* game_objw1; game_objw1 = new GameObject(pos_ptrw1,
    //    collider_ptrw1, vis_ptrw1, "wall");
    //    //render.AddToRender(game_obj3, bebra_vis3);
    //    collision_system.RegisterColliderOf(game_objw1, bebra_colw1);
    //
    //    Position posw2(1200-5, 380);
    //    std::unique_ptr<VisibleObject> vis_ptrw2 = nullptr;
    //    auto bebra_visw2 = nullptr;
    //    auto pos_ptrw2 = std::make_unique<Position>(posw2);
    //    std::unique_ptr<Collider> collider_ptrw2 =
    //    std::make_unique<RectangleCollider>(Position(1200-5, 380), 10, 760, true); auto
    //    bebra_colw2 = collider_ptrw2.get(); GameObject* game_objw2; game_objw2 = new
    //    GameObject(pos_ptrw2, collider_ptrw2, vis_ptrw2, "wall");
    //    //render.AddToRender(game_obj3, bebra_vis3);
    //    collision_system.RegisterColliderOf(game_objw2, bebra_colw2);
    //
    //    Position posw3(600, 0);
    //    std::unique_ptr<VisibleObject> vis_ptrw3 = nullptr;
    //    auto bebra_visw3 = nullptr;
    //    auto pos_ptrw3 = std::make_unique<Position>(posw3);
    //    std::unique_ptr<Collider> collider_ptrw3 =
    //    std::make_unique<RectangleCollider>(Position(600, 0), 1200, 10, true); auto bebra_colw3 =
    //    collider_ptrw3.get(); GameObject* game_objw3; game_objw3 = new GameObject(pos_ptrw3,
    //    collider_ptrw3, vis_ptrw3, "wall");
    //    //render.AddToRender(game_obj3, bebra_vis3);
    //    collision_system.RegisterColliderOf(game_objw3, bebra_colw3);
    //
    //
    //    Position posw4(600, 760);
    //    std::unique_ptr<VisibleObject> vis_ptrw4 = nullptr;
    //    auto bebra_visw4 = nullptr;
    //    auto pos_ptrw4 = std::make_unique<Position>(posw4);
    //    std::unique_ptr<Collider> collider_ptrw4 =
    //    std::make_unique<RectangleCollider>(Position(600, 760), 1200, 10, true); auto bebra_colw4
    //    = collider_ptrw4.get(); GameObject* game_objw4; game_objw4 = new GameObject(pos_ptrw4,
    //    collider_ptrw4, vis_ptrw4, "wall");
    //    //render.AddToRender(game_obj3, bebra_vis3);
    //    collision_system.RegisterColliderOf(game_objw4, bebra_colw4);
    //
    time.ResetTime();

    const double SPEED = 10;

    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        auto temp = CringeMovement();
        if (!temp.empty()) {
            for (auto& vec : temp) {
                game_obj->Translate(vec * SPEED);
                auto collisions = engine.GetAllCollisions(game_obj);
                for (const auto& coll_info : collisions) {
                    if (coll_info.tag == "wall") {
                        game_obj->Translate(vec * (-SPEED));
                    }
                }
            }
        }
        engine.RenderAll();
    }

    return 0;
}
