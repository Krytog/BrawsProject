#include "LudusMapClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <Core/Tools/Concepts.h>
#include <cmath>

#include "WallCoordinates.h"

#define MAP_WIDTH 16000
#define MAP_HEIGHT 8667

#define VANISH_IMPL(X)
#define VANISH(X) VANISH_IMPL(X)

#define GetRes(x, y) RES_PATH_MAPS_LUDUS_background ## x ## y
#define Wall(x) CreateWall(MACRO_CONCAT(MACRO_CONCAT(WALL, x), _X), MACRO_CONCAT(MACRO_CONCAT(WALL, x), _Y), MACRO_CONCAT(MACRO_CONCAT(WALL, x), _WIDTH), MACRO_CONCAT(MACRO_CONCAT(WALL, x), _HEIGHT));

namespace {
    Position QuarterPosition(uint8_t x, uint8_t y) {
        const double coef_x = 0.25 * x - 0.375;
        const double coef_y = 0.25 * (3 - y) - 0.375;
        return Position(MAP_WIDTH * coef_x, MAP_HEIGHT * coef_y);
    }

    void CreateQuarter(uint8_t x, uint8_t y, std::string_view path) {
        auto pos = QuarterPosition(x, y);
        ClientEngine::GetInstance().
        CreateGameObject<PersistentVisibleObject>(new Position(pos), nullptr,
                                                  new BasicSprite(path, pos, std::ceil(MAP_WIDTH / 4) + 5, std::ceil(MAP_HEIGHT / 4) + 5,
                                                                  0, IVisibleObject::RenderLayers::BackGround), TAGS_MAPS_Ludus);
    }

    void CreateWall(double x, double y, double width, double height) {
        VisibleGameObject::ArgPack arg_pack;
        arg_pack.position = new Position(x, y);
        arg_pack.collider = new RectangleCollider(Position(x, y), width, height, Collider::Category::Ordinary);
        arg_pack.visible_object = nullptr;
        ClientEngine::GetInstance().CreateGameObject<PersistentVisibleObject>(arg_pack);
    }

    void CreateWalls() {
        Wall(1);
        Wall(2);
        Wall(3);
        Wall(4);
        Wall(5);
        Wall(6);
        Wall(7);
        Wall(8);
        Wall(9);
        Wall(10);
        Wall(11);
        Wall(12);
        Wall(13);
        Wall(14);
        Wall(15);
        Wall(16);
        Wall(17);
        Wall(18);
    }
}

LudusMapClient::LudusMapClient(): PersistentVisibleObject(new Position(0, 0), nullptr, nullptr, TAGS_MAPS_Ludus) {
    //CreateQuarter(0, 0, GetRes(0 , 0));
    //CreateQuarter(0, 1, GetRes(0 , 1));
    //CreateQuarter(0, 2, GetRes(0 , 2));
    //CreateQuarter(0, 3, GetRes(0 , 3));
    //CreateQuarter(1, 0, GetRes(1 , 0));
    //CreateQuarter(1, 1, GetRes(1 , 1));
    //CreateQuarter(1, 2, GetRes(1 , 2));
    //CreateQuarter(1, 3, GetRes(1 , 3));
    //CreateQuarter(2, 0, GetRes(2 , 0));
    //CreateQuarter(2, 1, GetRes(2 , 1));
    //CreateQuarter(2, 2, GetRes(2 , 2));
    //CreateQuarter(2, 3, GetRes(2 , 3));
    //CreateQuarter(3, 0, GetRes(3 , 0));
    //CreateQuarter(3, 1, GetRes(3 , 1));
    //CreateQuarter(3, 2, GetRes(3 , 2));
    //CreateQuarter(3, 3, GetRes(3 , 3));
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_MAPS_LUDUS_backgroundWHOLE, *position_, MAP_WIDTH, MAP_HEIGHT, 0, IVisibleObject::RenderLayers::BackGround);
    CreateWalls();
}