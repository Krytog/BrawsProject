#include "PowerUpPawnClient.h"

PowerUpPawnClient::PowerUpPawnClient() = default;
PowerUpPawnClient::~PowerUpPawnClient() = default;

void PowerUpPawnClient::UpdateRotation(const Vector2D &rotator) {
    visible_object_->UpdateRotation(rotator);
}