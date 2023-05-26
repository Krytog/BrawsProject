#pragma once

#include "PowerUpPawnClient.h"

class SpeedPowerUpPawnClient : public PowerUpPawnClient {
public:
    SpeedPowerUpPawnClient();
    ~SpeedPowerUpPawnClient();

    static const size_t kTypeId;
};
