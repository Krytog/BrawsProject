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
    COLLIDER_RADIUS = 70,
};

#define START_HEALTH 150
#define START_DAMAGE 25
#define START_SPEED 7.5
#define PROJECTILE_SPEED 15
#define START_AMMO 8
#define COOLDOWN_DURATION 2

#define SCALE 0.390625
#define BASIC_ANGLE (M_PI / 2)
static const Vector2D kShootPos(152 * SCALE, -171 * SCALE);

CharacterMagePawnServer::CharacterMagePawnServer() : CharacterMagePawnServer(Position(START_X, START_Y)) {}

CharacterMagePawnServer::CharacterMagePawnServer(const Position &position) : CharacterMagePawnServer(CharacterPawnServer::ArgPack(), position) {
    health_cur_ = START_HEALTH;
    health_max_ = START_HEALTH;
    damage_ = START_DAMAGE;
    speed_ = START_SPEED;
    ammo_cur_ = START_AMMO;
    ammo_max_ = START_AMMO;
    cooldown_duration_ = COOLDOWN_DURATION;
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
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id);
}

void CharacterMagePawnServer::Shoot(const Position &position) {
    if (!can_shoot_) {
        return;
    }
    if (ammo_cur_ == 0) {
        return;
    }
    CharacterPawnServer::Shoot(position);
    ProjectilePawnServer::ArgPack argpack;
    argpack.position = *position_;
    argpack.position.Translate(kShootPos.Rotate(direction_));
    argpack.damage = damage_;
    argpack.speed = PROJECTILE_SPEED;
    argpack.direction = (position.GetCoordinatesAsVector2D() - argpack.position.GetCoordinatesAsVector2D());
    argpack.direction.Normalize();
    ServerEngine::GetInstance().CreateGameObject<ProjectileMagePawnServer>(argpack);
}

void CharacterMagePawnServer::OnUpdate() {
    CharacterPawnServer::OnUpdate();
}

const size_t CharacterMagePawnServer::kTypeId = TypeId_Character_Mage;