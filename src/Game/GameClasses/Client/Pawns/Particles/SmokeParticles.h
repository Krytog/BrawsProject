#pragma once

#include "Particles.h"

class SmokeParticles : public Particles {
public:
    SmokeParticles();
    SmokeParticles(const Position& position);
    ~SmokeParticles();

    static const size_t kTypeId;
};
