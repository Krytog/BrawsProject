#include "CharacterDefaultPawnServer.h"

#include <Game/GameClasses/Server/Cerebrates/Characters/CharacterCerebrateServer.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>

enum {
    START_X = 0,
    START_Y = 0,
    COLLIDER_RADIUS = 35,
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100
};

CharacterDefaultPawnServer::CharacterDefaultPawnServer() : CharacterDefaultPawnServer(Position(START_X, START_Y)) {}

CharacterDefaultPawnServer::CharacterDefaultPawnServer(const Position &position) {
    cerebrate_id = Overmind::GetInstance().CreateCerebrateToPossess<CharacterCerebrateServer<CharacterDefaultPawnServer>>(this);
    auto pos = new Position(position);
    position_ = std::unique_ptr<Position>(pos);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    visible_object_ = nullptr;
    tag_ = TAGS_CHARACTER_Default;
}

CharacterDefaultPawnServer::~CharacterDefaultPawnServer() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id);
}

void CharacterDefaultPawnServer::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void CharacterDefaultPawnServer::OnUpdate() {
    Translate(Vector2D(100, 100));
}

const size_t CharacterDefaultPawnServer::kTypeId = TypeId_Character_Default;