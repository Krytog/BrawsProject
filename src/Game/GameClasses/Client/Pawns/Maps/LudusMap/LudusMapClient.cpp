#include "LudusMapClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <Core/Tools/Concepts.h>
#include <cmath>

#define MAP_WIDTH 16000
#define MAP_HEIGHT 8667

#define GetRes(x, y) RES_PATH_MAPS_LUDUS_background ## x ## y

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
        std::cout << pos.GetCoordinates().first << " " << pos.GetCoordinates().second << std::endl;
    }
}

LudusMapClient::LudusMapClient(): PersistentVisibleObject(new Position(0, 0), nullptr, nullptr, TAGS_MAPS_Ludus) {
    //CreateQuarter(0, 0, GetRes(0 , 0));
    //CreateQuarter(0, 1, GetRes(0 , 1));
    //CreateQuarter(0, 2, GetRes(0 , 2));
    //CreateQuarter(0, 3, GetRes(0 , 3));
    //CreateQuarter(1, 0, GetRes(1 , 0));
    CreateQuarter(1, 1, GetRes(1 , 1));
    CreateQuarter(1, 2, GetRes(1 , 2));
    //CreateQuarter(1, 3, GetRes(1 , 3));
    //CreateQuarter(2, 0, GetRes(2 , 0));
    CreateQuarter(2, 1, GetRes(2 , 1));
    CreateQuarter(2, 2, GetRes(2 , 2));
    //CreateQuarter(2, 3, GetRes(2 , 3));
    //CreateQuarter(3, 0, GetRes(3 , 0));
    //CreateQuarter(3, 1, GetRes(3 , 1));
    //CreateQuarter(3, 2, GetRes(3 , 2));
    //CreateQuarter(3, 3, GetRes(3 , 3));
}