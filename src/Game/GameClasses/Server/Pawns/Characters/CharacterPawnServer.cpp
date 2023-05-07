#include "CharacterPawnServer.h"

#define FIELD_OF_VIEW_WIDTH 1280
#define FIELD_OF_VIEW_HEIGHT 720
#define FIELD_OF_VIEW_TAG "view_field"

CharacterPawnServer::CharacterPawnServer(): CharacterPawnServer(ArgPack()) {}

CharacterPawnServer::~CharacterPawnServer() {
    Engine::GetInstance().Destroy(field_of_view_);
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
    args.visible_object = nullptr;
    args.tag = FIELD_OF_VIEW_TAG;
    auto ptr = Engine::GetInstance().CreateGameObject<PersistentObject>(args);
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