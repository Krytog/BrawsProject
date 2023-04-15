#include "DefaultCharacterPawnClient.h"

#include "../../../../../Resources/ResourcesPaths.h"
#include "../../../Cerebrates/Characters/CharacterCerebrateClient.h"

#define TAG "default_character"

enum {
    START_X = 0,
    START_Y = 0,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

DefaultCharacterPawnClient::DefaultCharacterPawnClient() {
    new CharacterCerebrateClient(this, false); // no memory leaks, overmind takes care of it
    position_ = std::make_unique<Position>(START_X, START_Y);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<StaticSprite>(*position_, SPRITE_WIDTH, SPRITE_HEIGHT, RES_PATH_CHARACTERS_DEFAULTCHARACTER_1, LEVELS::FIRST_USER_LEVEL);
    tag_ = TAG;
}

DefaultCharacterPawnClient::DefaultCharacterPawnClient(const Position &position) {
    new CharacterCerebrateClient(this, false); // no memory leaks, overmind takes care of it
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<StaticSprite>(*position_, SPRITE_WIDTH, SPRITE_HEIGHT, RES_PATH_CHARACTERS_DEFAULTCHARACTER_1, LEVELS::FIRST_USER_LEVEL);
    tag_ = TAG;
}

void DefaultCharacterPawnClient::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void DefaultCharacterPawnClient::ReceiveDamage() {
    std::cout << "I receive damage!" << std::endl;
}

void DefaultCharacterPawnClient::OnUpdate() {
    Translate(Vector2D(100, 100));
}