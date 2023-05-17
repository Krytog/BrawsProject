#include "CharacterDefaultPawnClient.h"

#include <Core/Observer.h>
#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>
#include <Game/GameClasses/GameObjectTags.h>

#include <iostream>

enum {
    START_X = 0,
    START_Y = 0,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

CharacterDefaultPawnClient::CharacterDefaultPawnClient() : CharacterDefaultPawnClient(Position(START_X, START_Y)) {}

CharacterDefaultPawnClient::CharacterDefaultPawnClient(const Position &position) {
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<BasicFlexibleSprite>(RES_PATH_CHARACTERS_DEFAULT, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, 0, IVisibleObject::RenderLayers::Objects);
    tag_ = TAGS_CHARACTER_Default;
}

void CharacterDefaultPawnClient::Shoot() {
    std::cout << "I shoot!" << std::endl;
}

void CharacterDefaultPawnClient::ReceiveDamage() {
    std::cout << "I receive damage!" << std::endl;
}

void CharacterDefaultPawnClient::OnUpdate() {
}

const size_t CharacterDefaultPawnClient::kTypeId = TypeId_Character_Default;