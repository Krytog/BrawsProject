#include "CharacterPawnClient.h"

CharacterPawnClient::CharacterPawnClient() = default;
CharacterPawnClient::~CharacterPawnClient() = default;

void CharacterPawnClient::OnUpdate() {}

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

size_t CharacterPawnClient::GetTypeId() const {
    return type_id_;
}
