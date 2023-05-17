#include "MapsDefaultClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>

enum {
    LAKE_X = 0,
    LAKE_Y = 0,
    LAKE_WIDTH = 400,
    LAKE_HEIGHT = 600,
    MAP_WIDTH = 4000,
    MAP_HEIGHT = 2000
};

MapsDefaultClient::MapsDefaultClient(): PersistentVisibleObject(new Position(0, 0), nullptr, nullptr, TAGS_MAPS_Default) {
    RectangleCollider* lake_collider = new RectangleCollider(Position(LAKE_X, LAKE_Y), LAKE_WIDTH, LAKE_HEIGHT);
    IVisibleObject* visible_object = new BasicSprite(RES_PATH_MAPS_DEFAULT_BACKGROUND, Position(0, 0), MAP_WIDTH, MAP_HEIGHT, 0, IVisibleObject::RenderLayers::BackGround);
    PersistentVisibleObject::ArgPack pack;
    pack.position = nullptr;
    pack.visible_object = nullptr;
    pack.collider = lake_collider;
    pack.tag = TAGS_COLLIDERS_NOWALK_FLY;
    ClientEngine::GetInstance().CreateGameObject<PersistentVisibleObject>(pack);
    visible_object_ = std::unique_ptr<IVisibleObject>(visible_object);
}