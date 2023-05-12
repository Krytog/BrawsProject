#include "CharacterMagePawnServer.h"

#include <Game/GameClasses/Server/Cerebrates/Characters/CharacterCerebrateServer.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/Server/Pawns/Characters/Mage/ProjectileMagePawnServer.h>

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
#define PROJECTILE_SPEED 15

static const Vector2D kShootPos(100, 100);

CharacterMagePawnServer::CharacterMagePawnServer() : CharacterMagePawnServer(Position(START_X, START_Y)) {}

CharacterMagePawnServer::CharacterMagePawnServer(const Position &position) : CharacterMagePawnServer(CharacterPawnServer::ArgPack(), position) {
    health_ = START_HEALTH;
    damage_ = START_DAMAGE;
    speed_ = START_SPEED;
}

CharacterMagePawnServer::CharacterMagePawnServer(const CharacterPawnServer::ArgPack& base_arg_pack, const Position &position):
        CharacterPawnServer(base_arg_pack) {
    cerebrate_id = Overmind::GetInstance().CreateCerebrateToPossess<CharacterCerebrateServer<CharacterMagePawnServer>>(this);
    position_ = std::make_unique<Position>(position);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    tag_ = TAGS_CHARACTER_Mage;
    CharacterPawnServer::UpdatePosition(position);
}

CharacterMagePawnServer::~CharacterMagePawnServer() {
    Overmind::GetInstance().GetCerebrateWithId(cerebrate_id)->MarkDeprecated();
}

void CharacterMagePawnServer::Shoot(const Position &position) {
    CharacterPawnServer::Shoot(position);
    ProjectilePawnServer::ArgPack argpack;
    argpack.position = *position_;
    argpack.position.Translate(kShootPos);
    argpack.damage = damage_;
    argpack.speed = PROJECTILE_SPEED;
    argpack.direction = (position.GetCoordinatesAsVector2D() - argpack.position.GetCoordinatesAsVector2D());
    ServerEngine::GetInstance().CreateGameObject<ProjectileMagePawnServer>(argpack);
}

void CharacterMagePawnServer::OnUpdate() {
    //Shoot(Position());
}

const size_t CharacterMagePawnServer::kTypeId = TypeId_Character_Mage;