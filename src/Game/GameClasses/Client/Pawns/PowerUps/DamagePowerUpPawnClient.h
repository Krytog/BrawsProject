#pragma once

#include "PowerUpPawnClient.h"

class DamagePowerUpPawnClient : public PowerUpPawnClient {
public:
    DamagePowerUpPawnClient();
    ~DamagePowerUpPawnClient();

    static const size_t kTypeId;
};
