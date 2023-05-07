#include "CharacterMagePawnServer.h"

#include <Game/GameClasses/Server/Cerebrates/Characters/CharacterCerebrateServer.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>

#include <iostream>

enum {
    START_X = -500,
    START_Y = -400,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

#define START_HEALTH 100
#define START_DAMAGE 25
#define START_SPEED 10

CharacterMagePawnServer::CharacterMagePawnServer() : CharacterMagePawnServer(Position(START_X, START_Y)) {}

CharacterMagePawnServer::CharacterMagePawnServer(const Position &position) : CharacterMagePawnServer(CharacterPawnServer::ArgPack(), position) {
    health_ = START_HEALTH;
    damage_ = START_DAMAGE;
    speed_ = START_SPEED;
}

CharacterMagePawnServer::CharacterMagePawnServer(const CharacterPawnServer::ArgPack& base_arg_pack, const Position &position):
        CharacterPawnServer(base_arg_pack) {
    cerebrate_id = Overmind::GetInstance().CreateCerebrateToPossess<CharacterCerebrateServer<CharacterMagePawnServer>>(this);
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = nullptr;
    tag_ = TAGS_CHARACTER_Mage;
    CharacterPawnServer::UpdatePosition(position);
}

CharacterMagePawnServer::~CharacterMagePawnServer() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id);
}

void CharacterMagePawnServer::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void CharacterMagePawnServer::OnUpdate() {
    //std::cout << "Mage: " << position_->GetCoordinates().first << " " << position_->GetCoordinates().second << std::endl;
}

const size_t CharacterMagePawnServer::kTypeId = TypeId_Character_Mage;