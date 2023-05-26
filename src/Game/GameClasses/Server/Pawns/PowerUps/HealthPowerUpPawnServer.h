#pragma once

#include "PowerUpPawnServer.h"


class HealthPowerUpPawnServer : public PowerUpPawnServer {
public:
    HealthPowerUpPawnServer(const Position& position);

    static const size_t kTypeId;
private:
    void ApplyEffect(CharacterPawnServer* character) override;
};
