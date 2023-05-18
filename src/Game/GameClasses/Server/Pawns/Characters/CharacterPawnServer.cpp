#include "CharacterPawnServer.h"
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/CommandsList.h>
#include <Game/GameClasses/Server/GameRulers/GameRuler.h>

#define FIELD_OF_VIEW_TAG "view_field"


CharacterPawnServer::CharacterPawnServer(): CharacterPawnServer(ArgPack()) {}

CharacterPawnServer::~CharacterPawnServer() {
    ServerEngine::GetInstance().Destroy(field_of_view_);
    ServerEngine::GetInstance().DestroyEvent(is_dead_event_);
}

CharacterPawnServer::CharacterPawnServer(const CharacterPawnServer::ArgPack &arg_pack):
    health_cur_(arg_pack.health_max),
    health_max_(arg_pack.health_max),
    damage_(arg_pack.damage),
    speed_(arg_pack.speed),
    ammo_cur_(arg_pack.ammo_max),
    ammo_max_(arg_pack.ammo_max),
    cooldown_duration_(arg_pack.cooldown_duration) {
    GameObject::ArgPack args;
    args.position = new Position(*position_);
    args.collider = new RectangleCollider(*position_, FIELD_OF_VIEW_WIDTH, FIELD_OF_VIEW_HEIGHT, Collider::Category::Technical);
    args.tag = FIELD_OF_VIEW_TAG;
    auto ptr = ServerEngine::GetInstance().CreateGameObject<PersistentObject>(args);
    field_of_view_ = ptr;
    is_dead_event_ = CreateIsDeadEvent();
    GameRuler::GetInstance().AddPlayer(this);
}

Event* CharacterPawnServer::CreateIsDeadEvent() const {
    return ServerEngine::GetInstance().CreateEvent(&CharacterPawnServer::IsDead, std::make_tuple(this),
                                            &GameRuler::PlayerLoses, std::make_tuple(&GameRuler::GetInstance(),
                                            this), EventStatus::Disposable);
}

size_t CharacterPawnServer::GetCerebrateId() const {
    return cerebrate_id;
}

void CharacterPawnServer::Translate(const Vector2D& vector2d) {
    GameObject::Translate(vector2d);
    field_of_view_->Translate(vector2d);
}

void CharacterPawnServer::UpdatePosition(const Position &position) {
    GameObject::UpdatePosition(position);
    field_of_view_->UpdatePosition(position);
}

GameObject *CharacterPawnServer::GetFieldOfView() const {
    return field_of_view_;
}

void CharacterPawnServer::Move(const Vector2D &direction) {
    MoveImplementation(direction * speed_);
}

void CharacterPawnServer::MoveImplementation(const Vector2D &direction) {
    Vector2D projections[4];
    projections[0] = Vector2D::Up * (direction * Vector2D::Up);
    projections[1] = Vector2D::Down * (direction * Vector2D::Down);
    projections[2] = Vector2D::Right * (direction * Vector2D::Right);
    projections[3] = Vector2D::Left * (direction * Vector2D::Left);
    for (int i = 0; i < 4; ++i) {
        if (projections[i].IsZero()) {
            continue;
        }
        collider_->Translate(projections[i]);
        auto collisions = ServerEngine::GetInstance().GetPhysicalCollisions(this);
        collider_->Translate(projections[i] * -1);
        if (collisions.empty()) {
            Translate(projections[i]);
        }
    }
}

void CharacterPawnServer::ReceiveDamage(double damage) {
    health_cur_ -= damage;
    heal_timer_.ResetTime();
    std::string command;
    command.push_back(CharacterCommands::COMMAND_RECEIVE_DAMAGE);
    Overmind::GetInstance().GetCerebrateWithId(cerebrate_id)->AddCommandToBuffer(command);
}

void CharacterPawnServer::Shoot(const Position &position) {
    if (ammo_cur_ == 0) {
        return;
    }
    --ammo_cur_;
    cooldown_timer_.ResetTime();
    std::string command;
    command.push_back(CharacterCommands::COMMAND_SHOOT);
    Overmind::GetInstance().GetCerebrateWithId(cerebrate_id)->AddCommandToBuffer(command);
}

void CharacterPawnServer::Heal(double health) {
    health_cur_ = std::min(health_max_, health_cur_ + health);
}

double CharacterPawnServer::GetHealthCur() const {
    return health_cur_;
}

double CharacterPawnServer::GetHealthMax() const {
    return health_max_;
}

uint8_t CharacterPawnServer::GetAmmoLeft() const {
    return ammo_cur_;
}

uint8_t CharacterPawnServer::GetAmmoMax() const {
    return ammo_max_;
}

double CharacterPawnServer::GetCooldownDuration() const {
    return cooldown_duration_;
}

double CharacterPawnServer::GetCooldown() const {
    if (ammo_cur_ == ammo_max_) {
        return 0.0;
    }
    return std::max(cooldown_duration_ - cooldown_timer_.EvaluateTime(), 0.0);
}

double CharacterPawnServer::GetDamage() const {
    return damage_;
}

double CharacterPawnServer::GetSpeed() const {
    return speed_;
}

void CharacterPawnServer::BlockShooting() {
    can_shoot_ = false;
}

void CharacterPawnServer::UnblockShooting() {
    can_shoot_ = true;
}

void CharacterPawnServer::SetDirection(const Position& looking_at) {
    Vector2D direction = Position::GetDirection(*position_, looking_at);
    direction_ = direction;
}

void CharacterPawnServer::SetHealthCur(double health) {
    health_cur_ = health;
}

void CharacterPawnServer::SetHealthMax(double max_health) {
    health_max_ = max_health;
}

void CharacterPawnServer::SetAmmoLeft(uint8_t ammo_left) {
    ammo_cur_ = ammo_left;
}

void CharacterPawnServer::SetAmmoMax(uint8_t ammo_max) {
    ammo_max_ = ammo_max;
}

void CharacterPawnServer::SetCooldownDuration(double duration) {
    cooldown_duration_ = duration;
}

void CharacterPawnServer::SetDamage(double damage) {
    damage_ = damage;
}

void CharacterPawnServer::SetSpeed(double speed) {
    speed_ = speed;
}

bool CharacterPawnServer::IsDead() const {
    return health_cur_ <= 0;
}

void CharacterPawnServer::Reload() {
    if (ammo_cur_ < ammo_max_) {
        ++ammo_cur_;
    }
    cooldown_timer_.ResetTime();
}

#define PRECISION 0.005

void CharacterPawnServer::HandleReloading() {
    if (GetCooldown() < PRECISION) {
        Reload();
    }
}

#define HEAL_COOLDOWN 3

void CharacterPawnServer::HandleOverTimeHealing() {
    if (heal_timer_.EvaluateTime() > HEAL_COOLDOWN) {
        Heal(heal_rate_ * health_max_);
        heal_timer_.ResetTime();
    }
}

Vector2D CharacterPawnServer::GetDirection() const {
    return direction_;
}

void CharacterPawnServer::OnUpdate() {
    HandleReloading();
    HandleOverTimeHealing();
}