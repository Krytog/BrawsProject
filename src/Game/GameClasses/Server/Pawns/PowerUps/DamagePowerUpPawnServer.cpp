#include "DamagePowerUpPawnServer.h"
#include <Game/GameClasses/Server/Cerebrates/Positional/PositionalCerebrateServer.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    COLLIDER_WIDTH = 80,
    COLLIDER_HEIGHT = 80
};

#define BOOST_RATE 1.25

DamagePowerUpPawnServer::DamagePowerUpPawnServer(const Position& position) {
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<PositionalCerebrateServer<DamagePowerUpPawnServer>>(this);
    collider_ = std::make_unique<RectangleCollider>(position, COLLIDER_WIDTH, COLLIDER_HEIGHT, Collider::Category::Trigger);
    GameObject::UpdatePosition(position);
    tag_ = TAGS_POWERUP_DAMAGE;
}

void DamagePowerUpPawnServer::ApplyEffect(CharacterPawnServer* character) {
    auto damage = character->GetDamage();
    character->SetDamage(damage * BOOST_RATE);
}

const size_t DamagePowerUpPawnServer::kTypeId = TypeId_Powerup_Damage;