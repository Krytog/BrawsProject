#include "HealthPowerUpPawnServer.h"
#include <Game/GameClasses/Server/Cerebrates/Positional/PositionalCerebrateServer.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    COLLIDER_WIDTH = 80,
    COLLIDER_HEIGHT = 80
};

#define BOOST_RATE 1.2

HealthPowerUpPawnServer::HealthPowerUpPawnServer(const Position& position) {
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<PositionalCerebrateServer<HealthPowerUpPawnServer>>(this);
    collider_ = std::make_unique<RectangleCollider>(position, COLLIDER_WIDTH, COLLIDER_HEIGHT, Collider::Category::Trigger);
    GameObject::UpdatePosition(position);
    tag_ = TAGS_POWERUP_HEALTH;
}

void HealthPowerUpPawnServer::ApplyEffect(CharacterPawnServer *character) {
    auto cur_health = character->GetHealthCur();
    auto max_health = character->GetHealthMax();
    character->SetHealthMax(max_health * BOOST_RATE);
    character->SetHealthCur(cur_health * BOOST_RATE);
}

const size_t HealthPowerUpPawnServer::kTypeId = TypeId_Powerup_Health;