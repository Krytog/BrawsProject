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

int8_t CharacterPawnClient::GetAmmoLeft() const {
    return ammo_left_;
}

double CharacterPawnClient::GetHealth() const {
    return health_;
}

double CharacterPawnClient::GetCooldown() const {
    return cooldown_;
}

void CharacterPawnClient::SetHealth(double health) {
    health_ = health;
}

void CharacterPawnClient::SetAmmoLeft(int8_t ammo) {
    ammo_left_ = ammo;
}

void CharacterPawnClient::SetCooldown(double cooldown) {
    cooldown_ = cooldown;
}

void CharacterPawnClient::CaptureViewPort() {
    ClientEngine::GetInstance().SetCameraOn(this);
    is_controlled_ = true;
}

void CharacterPawnClient::SetRotation(const Vector2D &rotator) {
    visible_object_->UpdateRotation(rotator);
}
