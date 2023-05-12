#include "MapsDefaultClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>

enum {
    MAP_WIDTH = 4000,
    MAP_HEIGHT = 2000
};

MapsDefaultClient::MapsDefaultClient(): PersistentVisibleObject(new Position(0, 0), nullptr, nullptr, TAGS_MAPS_Default) {
    IVisibleObject* visible_object = new BasicSprite(RES_PATH_MAPS_DEFAULT_BACKGROUND, Position(0, 0), MAP_WIDTH, MAP_HEIGHT, 0, IVisibleObject::RenderLayers::BackGround);
    visible_object_ = std::unique_ptr<IVisibleObject>(visible_object);
}