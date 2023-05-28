#pragma once

#include "Particles.h"

class HitParticles : public Particles {
public:
    HitParticles();
    HitParticles(const Position& position);
    ~HitParticles();

    static const size_t kTypeId;
};
