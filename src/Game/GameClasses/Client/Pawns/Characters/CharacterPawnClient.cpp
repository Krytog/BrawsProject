#include "CharacterPawnClient.h"

CharacterPawnClient::CharacterPawnClient() = default;
CharacterPawnClient::~CharacterPawnClient() = default;

void CharacterPawnClient::OnUpdate() {
    if (is_controlled_) {
        auto input = ClientEngine::GetInstance().GetInput();
        auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
        Vector2D rotator = mouse_token.position.GetCoordinatesAsVector2D() - position_->GetCoordinatesAsVector2D();
        visible_object_->UpdateRotation(rotator);
    }
}

double CharacterPawnClient::GetDamage() const {
    return damage_;
}

double CharacterPawnClient::GetHealth() const {
    return health_;
}

double CharacterPawnClient::GetSpeed() const {
    return speed_;
}

void CharacterPawnClient::SetHealth(double health) {
    health_ = health;
}

void CharacterPawnClient::SetDamage(double damage) {
    damage_ = damage;
}

void CharacterPawnClient::SetSpeed(double speed) {
    speed_ = speed;
}

void CharacterPawnClient::CaptureViewPort() {
    ClientEngine::GetInstance().SetCameraOn(this);
    is_controlled_ = true;
}

void CharacterPawnClient::SetRotation(const Vector2D &rotator) {
    visible_object_->UpdateRotation(rotator);
}
