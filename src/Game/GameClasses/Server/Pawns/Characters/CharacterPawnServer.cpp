#include "CharacterPawnServer.h"
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/CommandsList.h>

#define FIELD_OF_VIEW_TAG "view_field"

CharacterPawnServer::CharacterPawnServer(): CharacterPawnServer(ArgPack()) {}

CharacterPawnServer::~CharacterPawnServer() {
    ServerEngine::GetInstance().Destroy(field_of_view_);
}

void CharacterPawnServer::OnUpdate() {}

double CharacterPawnServer::GetDamage() const {
    return damage_;
}

double CharacterPawnServer::GetHealth() const {
    return health_;
}

double CharacterPawnServer::GetSpeed() const {
    return speed_;
}

void CharacterPawnServer::SetHealth(double health) {
    health_ = health;
}

void CharacterPawnServer::SetDamage(double damage) {
    damage_ = damage;
}

void CharacterPawnServer::SetSpeed(double speed) {
    speed_ = speed;
}

CharacterPawnServer::CharacterPawnServer(const CharacterPawnServer::ArgPack &arg_pack):
    health_(arg_pack.health),
    damage_(arg_pack.damage),
    speed_(arg_pack.speed) {
    GameObject::ArgPack args;
    args.position = new Position(*position_);
    args.collider = new RectangleCollider(*position_, FIELD_OF_VIEW_WIDTH, FIELD_OF_VIEW_HEIGHT, Collider::Category::Technical);
    args.tag = FIELD_OF_VIEW_TAG;
    auto ptr = ServerEngine::GetInstance().CreateGameObject<PersistentObject>(args);
    field_of_view_ = ptr;
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
    IMovable::Move(direction * speed_);
}

void CharacterPawnServer::ReceiveDamage(double damage) {
    health_ -= damage;
    std::string command;
    command.push_back(CharacterCommands::COMMAND_RECEIVE_DAMAGE);
    Overmind::GetInstance().GetCerebrateWithId(cerebrate_id)->AddCommandToBuffer(command);
}

void CharacterPawnServer::Shoot(const Position &position) {
    std::string command;
    command.push_back(CharacterCommands::COMMAND_SHOOT);
    Overmind::GetInstance().GetCerebrateWithId(cerebrate_id)->AddCommandToBuffer(command);
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