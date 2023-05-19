#include "CharacterPiratePawnClient.h"

#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>
#include <Game/GameClasses/GameObjectTags.h>
#include "Core/Qt/PaintTemplates/BasicSprite.h"

enum {
    START_X = -50,
    START_Y = -50,
    COLLIDER_RADIUS = 70,
    SPRITE_WIDTH = 200,
    SPRITE_HEIGHT = 200
};

#define BASIC_STAT -1

CharacterPiratePawnClient::CharacterPiratePawnClient() : CharacterPiratePawnClient(Position(START_X, START_Y)) {}

CharacterPiratePawnClient::CharacterPiratePawnClient(const Position &position) {
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = std::make_unique<BasicFlexibleSprite>(RES_PATH_CHARACTERS_PIRATE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, IVisibleObject::RenderLayers::Objects);
    health_ = BASIC_STAT;
    cooldown_ = BASIC_STAT;
    ammo_left_ = BASIC_STAT;
    tag_ = TAGS_CHARACTER_Pirate;
    CharacterPawnClient::AddStatsVisualization();
}

void CharacterPiratePawnClient::Shoot() {}

void CharacterPiratePawnClient::ReceiveDamage() {
    std::cout << "I receive damage!" << std::endl;
}

void CharacterPiratePawnClient::OnUpdate() {
    CharacterPawnClient::OnUpdate();
}

const size_t CharacterPiratePawnClient::kTypeId = TypeId_Character_Pirate;

CharacterPiratePawnClient::~CharacterPiratePawnClient() {}