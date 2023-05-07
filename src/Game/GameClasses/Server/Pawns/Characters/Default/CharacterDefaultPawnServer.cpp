#include "CharacterDefaultPawnServer.h"

#include <Game/GameClasses/Server/Cerebrates/Characters/CharacterCerebrateServer.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>

#include <iostream>

enum {
    START_X = -500,
    START_Y = -500,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

#define START_HEALTH 100
#define START_DAMAGE 25
#define START_SPEED 10

CharacterDefaultPawnServer::CharacterDefaultPawnServer() : CharacterDefaultPawnServer(Position(START_X, START_Y)) {}

CharacterDefaultPawnServer::CharacterDefaultPawnServer(const Position &position) : CharacterDefaultPawnServer(CharacterPawnServer::ArgPack(), position) {
    health_ = START_HEALTH;
    damage_ = START_DAMAGE;
    speed_ = START_SPEED;
}

CharacterDefaultPawnServer::CharacterDefaultPawnServer(const CharacterPawnServer::ArgPack& base_arg_pack, const Position &position):
    CharacterPawnServer(base_arg_pack) {
    cerebrate_id = Overmind::GetInstance().CreateCerebrateToPossess<CharacterCerebrateServer<CharacterDefaultPawnServer>>(this);
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = nullptr;
    tag_ = TAGS_CHARACTER_Default;
    CharacterPawnServer::UpdatePosition(position);
}

CharacterDefaultPawnServer::~CharacterDefaultPawnServer() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id);
}

void CharacterDefaultPawnServer::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void CharacterDefaultPawnServer::OnUpdate() {
    std::cout << "DEFAULT: " << position_->GetCoordinates().first << " " << position_->GetCoordinates().second << std::endl;
}

const size_t CharacterDefaultPawnServer::kTypeId = TypeId_Character_Default;