#include "CharacterPawnServer.h"

CharacterPawnServer::CharacterPawnServer() = default;

CharacterPawnServer::~CharacterPawnServer() = default;

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
