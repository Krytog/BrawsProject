#include "SpeedPowerUpPawnServer.h"
#include <Game/GameClasses/Server/Cerebrates/Positional/PositionalCerebrateServer.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    COLLIDER_WIDTH = 80,
    COLLIDER_HEIGHT = 80
};

#define BOOST_RATE 1.1

SpeedPowerUpPawnServer::SpeedPowerUpPawnServer(const Position& position) {
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<PositionalCerebrateServer<SpeedPowerUpPawnServer>>(this);
    collider_ = std::make_unique<RectangleCollider>(position, COLLIDER_WIDTH, COLLIDER_HEIGHT, Collider::Category::Trigger);
    GameObject::UpdatePosition(position);
    tag_ = TAGS_POWERUP_SPEED;
}

void SpeedPowerUpPawnServer::ApplyEffect(CharacterPawnServer *character) {
    auto speed = character->GetSpeed();
    character->SetSpeed(speed * BOOST_RATE);
}

const size_t SpeedPowerUpPawnServer::kTypeId = TypeId_Powerup_Speed;