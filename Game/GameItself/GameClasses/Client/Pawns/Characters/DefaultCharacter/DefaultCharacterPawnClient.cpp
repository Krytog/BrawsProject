#include "DefaultCharacterPawnClient.h"

#include "../../../../../Resources/ResourcesPaths.h"
#include "../../../Cerebrates/Characters/CharacterCerebrateClient.h"
#include "../../../../../../SwarmSystem/TypeIdList.h"
#include "../../../../GameObjectTags.h"

enum {
    START_X = 0,
    START_Y = 0,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

DefaultCharacterPawnClient::DefaultCharacterPawnClient() : DefaultCharacterPawnClient(Position(START_X, START_Y)) {}

DefaultCharacterPawnClient::DefaultCharacterPawnClient(const Position &position) {
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<StaticSprite>(*position_, SPRITE_WIDTH, SPRITE_HEIGHT, RES_PATH_CHARACTERS_DEFAULTCHARACTER_1, LEVELS::FIRST_USER_LEVEL);
    tag_ = TAGS_CHARACTER_DefaultCharacter;
    type_id_ = TypeId_Character_DefaultCharacter;
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