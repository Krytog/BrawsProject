#include "CharacterMagePawnClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>
#include <Game/GameClasses/GameObjectTags.h>

#include <iostream>

enum {
    START_X = -50,
    START_Y = -50,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

CharacterMagePawnClient::CharacterMagePawnClient() : CharacterMagePawnClient(Position(START_X, START_Y)) {}

CharacterMagePawnClient::CharacterMagePawnClient(const Position &position) {
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_CHARACTERS_MAGE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, IVisibleObject::RenderLayers::Objects);
    tag_ = TAGS_CHARACTER_Mage;
}

void CharacterMagePawnClient::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void CharacterMagePawnClient::ReceiveDamage() {
    std::cout << "I receive damage!" << std::endl;
}

void CharacterMagePawnClient::OnUpdate() {
    //std::cout << position_->GetCoordinates().first << " " << position_->GetCoordinates().second << std::endl;
}

const size_t CharacterMagePawnClient::kTypeId = TypeId_Character_Mage;