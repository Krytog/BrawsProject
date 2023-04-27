#include "MapsDefaultClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <Game/GameClasses/GameObjectTags.h>

enum {
    MAP_WIDTH = 4000,
    MAP_HEIGHT = 2000
};

MapsDefaultClient::MapsDefaultClient(): PersistentObject(new Position(0, 0), nullptr, nullptr, TAGS_MAPS_Default) {
    VisibleObject* visible_object = new StaticSprite(Position(0, 0), MAP_WIDTH, MAP_HEIGHT, RES_PATH_MAPS_DEFAULT_BACKGROUND, LEVELS::BACKGROUND_LEVEL);
    visible_object_ = std::unique_ptr<VisibleObject>(visible_object);
}