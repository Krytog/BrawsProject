#include "SmokeParticles.h"

#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    SPRITE_WIDTH = 300,
    SPRITE_HEIGHT = 300,
    TICKS = 1,
    FRAMES = 15,
    CYCLED = false,
    ROTATION = 0
};

#define BEAUTIFUL_DESTROY_TIME 19

SmokeParticles::SmokeParticles() : SmokeParticles(Position()) {}

SmokeParticles::SmokeParticles(const Position &position) {
    Particles::ArgPack pack;
    pack.pos = position;
    pack.ticks = TICKS;
    pack.frames = FRAMES;
    pack.width = SPRITE_WIDTH;
    pack.height = SPRITE_HEIGHT;
    pack.rotation = ROTATION;
    pack.path = RES_PATH_PARTICLES_SMOKE;
    PrepareParticles(pack);
    MakeInvokeForDestroy(BEAUTIFUL_DESTROY_TIME);
}

SmokeParticles::~SmokeParticles() = default;

const size_t SmokeParticles::kTypeId = TypeId_Trail_Smoke;