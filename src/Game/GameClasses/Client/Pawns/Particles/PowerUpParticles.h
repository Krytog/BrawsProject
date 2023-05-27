#pragma once

#include "Particles.h"

class PowerUpParticles : public Particles {
public:
    PowerUpParticles();
    PowerUpParticles(const Position& position);
    ~PowerUpParticles();

    static const size_t kTypeId;
};
