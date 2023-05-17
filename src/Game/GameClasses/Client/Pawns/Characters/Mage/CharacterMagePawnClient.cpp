#include "CharacterMagePawnClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>
#include <Game/GameClasses/GameObjectTags.h>

#include <iostream>
#include "Core/Qt/PaintTemplates/BasicSprite.h"

enum {
    START_X = -50,
    START_Y = -50,
    COLLIDER_RADIUS = 70,
    SPRITE_WIDTH = 200,
    SPRITE_HEIGHT = 200
};

#define BASIC_STAT -1

CharacterMagePawnClient::CharacterMagePawnClient() : CharacterMagePawnClient(Position(START_X, START_Y)) {}

CharacterMagePawnClient::CharacterMagePawnClient(const Position &position) {
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<BasicFlexibleSprite>(RES_PATH_CHARACTERS_MAGE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, IVisibleObject::RenderLayers::Objects);
    health_ = BASIC_STAT;
    cooldown_ = BASIC_STAT;
    ammo_left_ = BASIC_STAT;
    tag_ = TAGS_CHARACTER_Mage;
    CharacterPawnClient::AddStatsVisualization();
}

void CharacterMagePawnClient::Shoot() {}

void CharacterMagePawnClient::ReceiveDamage() {
    std::cout << "I receive damage!" << std::endl;
}

void CharacterMagePawnClient::OnUpdate() {
    CharacterPawnClient::OnUpdate();
//    std::cout << "HEALTH: " << health_ << std::endl;
//    std::cout << "AMMO: " << ammo_left_ << std::endl;
//    std::cout << "COOLDOWN: " << cooldown_ << std::endl;
}

const size_t CharacterMagePawnClient::kTypeId = TypeId_Character_Mage;

CharacterMagePawnClient::~CharacterMagePawnClient() {}