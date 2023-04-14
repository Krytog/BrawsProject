#include "Map1.h"

#include "../../../Resources/ResourcesPaths.h"
#include "../../GameObjectTags.h"

enum {
    LAKE_X = 2000,
    LAKE_Y = 1000,
    LAKE_WIDTH = 400,
    LAKE_HEIGHT = 600,
    MAP_WIDTH = 4000,
    MAP_HEIGHT = 2000
};

#define MAP_TAG "background"

Map1::Map1(): GameObject(new Position(0, 0), nullptr, nullptr, MAP_TAG) {
    RectangleCollider* lake_collider = new RectangleCollider(Position(LAKE_X, LAKE_Y), LAKE_WIDTH, LAKE_HEIGHT);
    Engine::GetInstance().CreateGameObject<GameObject>(nullptr, lake_collider, nullptr, TAGS_COLLIDERS_NOWALK_FLY);
    VisibleObject* visible_object = new StaticSprite(Position(0, 0), MAP_WIDTH, MAP_HEIGHT, RES_PATH_MAPS_MAP1_BACKGROUND, LEVELS::BACKGROUND_LEVEL);
    visible_object_ = std::unique_ptr<VisibleObject>(visible_object);
}