#include "LudusMap.h"

#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/HealthPowerUpPawnServer.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/SpeedPowerUpPawnServer.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/DamagePowerUpPawnServer.h>
#include <Core/Tools/Concepts.h>

#include "ObjectsCoordinates.h"

namespace {
    void CreateWall(double x, double y, double width, double height) {
        GameObject::ArgPack arg_pack;
        arg_pack.position = new Position(x, y);
        arg_pack.collider = new RectangleCollider(Position(x, y), width, height);
        arg_pack.tag = TAGS_COLLIDERS_NOWALK_NOFLY;
        ServerEngine::GetInstance().CreateGameObject<PersistentObject>(arg_pack);
    }

    template<typename T>
    void CreatePowerUp(double x, double y) {
        static_assert(std::is_base_of<PowerUpPawnServer, T>(), "T must inherit from PowerUpPawnServer");
        ServerEngine::GetInstance().CreateGameObject<T>(Position(x, y));
    }
}

enum {
    MAP_WIDTH = 16000,
    MAP_HEIGHT = 8667,
};

#define VANISH_IMPL(X)
#define VANISH(X) VANISH_IMPL(X)

#define GetRes(x, y) RES_PATH_MAPS_LUDUS_background ## x ## y
#define Wall(x) CreateWall(MACRO_CONCAT(MACRO_CONCAT(WALL, x), _X), MACRO_CONCAT(MACRO_CONCAT(WALL, x), _Y), MACRO_CONCAT(MACRO_CONCAT(WALL, x), _WIDTH), MACRO_CONCAT(MACRO_CONCAT(WALL, x), _HEIGHT));

void LudusMapServer::SpawnPowerUps() {
    CreatePowerUp<HealthPowerUpPawnServer>(0, -800);
    CreatePowerUp<SpeedPowerUpPawnServer>(200, -800);
    CreatePowerUp<HealthPowerUpPawnServer>(-200, -800);
    CreatePowerUp<SpeedPowerUpPawnServer>(0, 0);
    CreatePowerUp<SpeedPowerUpPawnServer>(0, 500);
}

void LudusMapServer::SpawnWalls() {
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

LudusMapServer::LudusMapServer(): PersistentObject(new Position(0, 0), nullptr, TAGS_MAPS_Ludus) {
    SpawnPowerUps();
    SpawnWalls();
}

const size_t LudusMapServer::kHeight = MAP_HEIGHT;
const size_t LudusMapServer::kWidth = MAP_WIDTH;