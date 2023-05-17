#pragma once

#include "Particles.h"

class ExplosionParticles : public Particles {
public:
    ExplosionParticles();
    ExplosionParticles(const Position& position);
    ~ExplosionParticles();

    static const size_t kTypeId;
};
