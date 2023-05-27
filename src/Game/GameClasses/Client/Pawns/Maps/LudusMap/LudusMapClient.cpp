#include "LudusMapClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>

enum {
    MAP_WIDTH = 12000,
    MAP_HEIGHT = 6000
};

LudusMapClient::LudusMapClient(): PersistentVisibleObject(new Position(0, 0), nullptr, nullptr, TAGS_MAPS_Ludus) {
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_MAPS_DEFAULT_BACKGROUND, Position(0, 0), MAP_WIDTH, MAP_HEIGHT, 0, IVisibleObject::RenderLayers::BackGround);
}