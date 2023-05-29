#include "LudusMap.h"

#include <Game/GameClasses/GameObjectTags.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/HealthPowerUpPawnServer.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/SpeedPowerUpPawnServer.h>
#include <Game/GameClasses/Server/Pawns/PowerUps/DamagePowerUpPawnServer.h>

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

void LudusMapServer::SpawnPowerUps() {
    CreatePowerUp<HealthPowerUpPawnServer>(0, -800);
    CreatePowerUp<SpeedPowerUpPawnServer>(200, -800);
    CreatePowerUp<HealthPowerUpPawnServer>(-200, -800);
    CreatePowerUp<SpeedPowerUpPawnServer>(0, 0);
    CreatePowerUp<SpeedPowerUpPawnServer>(0, 500);
}

void LudusMapServer::SpawnWalls() {
    CreateWall(WALL1_X, WALL1_Y, WALL1_WIDTH, WALL1_HEIGHT);
    CreateWall(WALL2_X, WALL2_Y, WALL2_WIDTH, WALL2_HEIGHT);
    CreateWall(WALL3_X, WALL3_Y, WALL3_WIDTH, WALL3_HEIGHT);
    CreateWall(WALL4_X, WALL4_Y, WALL4_WIDTH, WALL4_HEIGHT);
    CreateWall(WALL5_X, WALL5_Y, WALL5_WIDTH, WALL5_HEIGHT);
    CreateWall(WALL6_X, WALL6_Y, WALL6_WIDTH, WALL6_HEIGHT);
    CreateWall(WALL7_X, WALL7_Y, WALL7_WIDTH, WALL7_HEIGHT);
    CreateWall(WALL8_X, WALL8_Y, WALL8_WIDTH, WALL8_HEIGHT);
}

LudusMapServer::LudusMapServer(): PersistentObject(new Position(0, 0), nullptr, TAGS_MAPS_Ludus) {
    SpawnPowerUps();
    SpawnWalls();
}

const size_t LudusMapServer::kHeight = MAP_HEIGHT;
const size_t LudusMapServer::kWidth = MAP_WIDTH;