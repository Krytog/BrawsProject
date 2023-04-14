#include "DefaultCharacterPawnClient.h"

#include "../../../../Resources/ResourcesPaths.h"

#define TAG "default_character"

enum {
    START_X = 0,
    START_Y = 0,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

DefaultCharacter::DefaultCharacter() {
    position_ = std::make_unique<Position>(START_X, START_Y);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<StaticSprite>(*position_, SPRITE_WIDTH, SPRITE_HEIGHT, RES_PATH_CHARACTERS_DEFAULTCHARACTER_1, LEVELS::FIRST_USER_LEVEL);
    tag_ = TAG;
}

DefaultCharacter::DefaultCharacter(const Position &position) {
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<StaticSprite>(*position_, SPRITE_WIDTH, SPRITE_HEIGHT, RES_PATH_CHARACTERS_DEFAULTCHARACTER_1, LEVELS::FIRST_USER_LEVEL);
    tag_ = TAG;
}

void DefaultCharacter::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void DefaultCharacter::ReceiveDamage() {
    std::cout << "I receive damage!" << std::endl;
}