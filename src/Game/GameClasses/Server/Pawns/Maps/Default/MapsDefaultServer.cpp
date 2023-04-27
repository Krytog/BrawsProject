#include "MapsDefaultServer.h"

#include <Game/GameClasses/GameObjectTags.h>

enum {
    LAKE_X = 2000,
    LAKE_Y = 1000,
    LAKE_WIDTH = 400,
    LAKE_HEIGHT = 600,
    MAP_WIDTH = 4000,
    MAP_HEIGHT = 2000
};

MapsDefaultServer::MapsDefaultServer(): PersistentObject(new Position(0, 0), nullptr, nullptr, TAGS_MAPS_Default) {
    RectangleCollider* lake_collider = new RectangleCollider(Position(LAKE_X, LAKE_Y), LAKE_WIDTH, LAKE_HEIGHT);
    Engine::GetInstance().CreateGameObject<PersistentObject>(nullptr, lake_collider, nullptr, TAGS_COLLIDERS_NOWALK_FLY);
}