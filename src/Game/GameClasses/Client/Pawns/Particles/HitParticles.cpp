#include "HitParticles.h"

#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    SPRITE_WIDTH = 300,
    SPRITE_HEIGHT = 250,
    TICKS = 1,
    FRAMES = 16,
    CYCLED = false,
    ROTATION = 0
};

#define BEAUTIFUL_DESTROY_TIME 50

HitParticles::HitParticles() : HitParticles(Position()) {}

HitParticles::HitParticles(const Position &position) {
    Particles::ArgPack pack;
    pack.pos = position;
    pack.ticks = TICKS;
    pack.frames = FRAMES;
    pack.width = SPRITE_WIDTH;
    pack.height = SPRITE_HEIGHT;
    pack.rotation = ROTATION;
    pack.path = RES_PATH_PARTICLES_HIT;
    PrepareParticles(pack);
    MakeInvokeForDestroy(BEAUTIFUL_DESTROY_TIME);
}

HitParticles::~HitParticles() = default;

const size_t HitParticles::kTypeId = TypeId_Trail_Hit;