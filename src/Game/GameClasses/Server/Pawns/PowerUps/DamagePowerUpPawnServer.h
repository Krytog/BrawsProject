#pragma once

#include "PowerUpPawnServer.h"

class DamagePowerUpPawnServer : public PowerUpPawnServer {
public:
    DamagePowerUpPawnServer(const Position& position);

    static const size_t kTypeId;
private:
    void ApplyEffect(CharacterPawnServer* character) override;
};
