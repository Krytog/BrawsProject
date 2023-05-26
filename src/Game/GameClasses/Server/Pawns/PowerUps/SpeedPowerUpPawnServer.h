#pragma once

#include "PowerUpPawnServer.h"

class SpeedPowerUpPawnServer : public PowerUpPawnServer {
public:
    SpeedPowerUpPawnServer(const Position& position);

    static const size_t kTypeId;
private:
    void ApplyEffect(CharacterPawnServer* character) override;
};
