#include "CharacterDefaultPawnServer.h"

#include <Game/GameClasses/Server/Cerebrates/Characters/CharacterCerebrateServer.h>
#include <Game/GameClasses/GameObjectTags.h>

enum {
    START_X = 0,
    START_Y = 0,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

DefaultCharacterPawnServer::DefaultCharacterPawnServer() : DefaultCharacterPawnServer(Position(START_X, START_Y)) {}

DefaultCharacterPawnServer::DefaultCharacterPawnServer(const Position &position) {
    new CharacterCerebrateServer<DefaultCharacterPawnServer>()
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = nullptr;
    tag_ = TAGS_CHARACTER_Default;
}

void DefaultCharacterPawnServer::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void DefaultCharacterPawnServer::OnUpdate() {
    Translate(Vector2D(100, 100));
}