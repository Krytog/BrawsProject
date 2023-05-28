#include "CharacterBullyPawnServer.h"

#include <Game/GameClasses/Server/Cerebrates/Characters/CharacterCerebrateServer.h>
#include <Game/GameClasses/GameObjectTags.h>
#include <SwarmSystem/TypeIdList.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/Server/Pawns/Characters/Bully/ProjectileBullyPawnServer.h>

#include <iostream>

enum {
    START_X = -500,
    START_Y = -400,
    COLLIDER_RADIUS = 70,
};

#define START_HEALTH 300
#define START_DAMAGE 75
#define START_SPEED 5.5
#define PROJECTILE_SPEED 15
#define START_AMMO 1
#define COOLDOWN_DURATION 1

#define SCALE 0.390625
#define BASIC_ANGLE (M_PI / 2)
static const Vector2D kShootPos(152 * SCALE, -171 * SCALE);

CharacterBullyPawnServer::CharacterBullyPawnServer() : CharacterBullyPawnServer(Position(START_X, START_Y)) {}

CharacterBullyPawnServer::CharacterBullyPawnServer(const Position &position) : CharacterBullyPawnServer(CharacterPawnServer::ArgPack(), position) {
    health_cur_ = START_HEALTH;
    health_max_ = START_HEALTH;
    damage_ = START_DAMAGE;
    speed_ = START_SPEED;
    ammo_cur_ = START_AMMO;
    ammo_max_ = START_AMMO;
    cooldown_duration_ = COOLDOWN_DURATION;
}

CharacterBullyPawnServer::CharacterBullyPawnServer(const CharacterPawnServer::ArgPack& base_arg_pack, const Position &position):
        CharacterPawnServer(base_arg_pack) {
    cerebrate_id = Overmind::GetInstance().CreateCerebrateToPossess<CharacterCerebrateServer<CharacterBullyPawnServer>>(this);
    position_ = std::make_unique<Position>(position);
    collider_ = std::make_unique<CircleCollider>(*position_, COLLIDER_RADIUS);
    tag_ = TAGS_CHARACTER_Bully;
    CharacterPawnServer::UpdatePosition(position);
}

CharacterBullyPawnServer::~CharacterBullyPawnServer() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id);
}

void CharacterBullyPawnServer::Shoot(const Position &position) {
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
    ServerEngine::GetInstance().CreateGameObject<ProjectileBullyPawnServer>(argpack);
}

void CharacterBullyPawnServer::OnUpdate() {
    CharacterPawnServer::OnUpdate();
}

const size_t CharacterBullyPawnServer::kTypeId = TypeId_Character_Bully;