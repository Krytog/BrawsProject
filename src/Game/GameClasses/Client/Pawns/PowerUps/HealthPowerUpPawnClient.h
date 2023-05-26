#pragma once

#include "PowerUpPawnClient.h"

class HealthPowerUpPawnClient : public PowerUpPawnClient {
public:
    HealthPowerUpPawnClient();
    ~HealthPowerUpPawnClient();

    static const size_t kTypeId;
};
