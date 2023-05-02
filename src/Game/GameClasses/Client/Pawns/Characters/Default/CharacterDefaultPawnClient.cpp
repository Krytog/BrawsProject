#include "CharacterDefaultPawnClient.h"

#include <Core/Observer.h>
#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <BotAPI/BotManagement.h>

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
    visible_object_ = std::make_unique<StaticSprite>(*position_, SPRITE_WIDTH, SPRITE_HEIGHT, RES_PATH_CHARACTERS_DEFAULT, LEVELS::FIRST_USER_LEVEL);
    tag_ = TAGS_CHARACTER_Default;
    /* if bot -> */ BotManagement::GetInstance().ObserveBot(this);
    Engine::GetInstance().SetCameraOn(this);
}

void CharacterDefaultPawnClient::Shoot(const Position &position) {
    std::cout << "I shoot!" << std::endl;
}

void CharacterDefaultPawnClient::ReceiveDamage() {
    std::cout << "I receive damage!" << std::endl;
}

void CharacterDefaultPawnClient::OnUpdate() {
    Translate(Vector2D(100, 100));
}

const size_t CharacterDefaultPawnClient::kTypeId = TypeId_Character_Default;