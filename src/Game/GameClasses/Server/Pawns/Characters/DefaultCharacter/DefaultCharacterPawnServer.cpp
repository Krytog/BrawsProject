#include "DefaultCharacterPawnServer.h"

#include "../../../../../Resources/ResourcesPaths.h"
#include "../../../Cerebrates/Characters/CharacterCerebrateServer.h"

#define TAG "default_character"

enum {
    START_X = 0,
    START_Y = 0,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

DefaultCharacterPawnServer::DefaultCharacterPawnServer() {
    position_ = std::make_unique<Position>(START_X, START_Y);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = nullptr;
    tag_ = TAG;
}

DefaultCharacterPawnServer::DefaultCharacterPawnServer(const Position &position) {
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = nullptr;
    tag_ = TAG;
}

void DefaultCharacterPawnServer::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void DefaultCharacterPawnServer::OnUpdate() {
    Translate(Vector2D(100, 100));
}