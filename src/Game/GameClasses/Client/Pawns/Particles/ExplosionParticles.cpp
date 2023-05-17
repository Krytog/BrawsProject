#include "ExplosionParticles.h"

#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    SPRITE_WIDTH = 100,
    SPRITE_HEIGHT = 100,
    TICKS = 1,
    FRAMES = 47,
    CYCLED = false,
    ROTATION = 0
};

#define BEAUTIFUL_DESTROY_TIME 47

ExplosionParticles::ExplosionParticles() : ExplosionParticles(Position()) {}

ExplosionParticles::ExplosionParticles(const Position &position) {
    Particles::ArgPack pack;
    pack.pos = position;
    pack.ticks = TICKS;
    pack.frames = FRAMES;
    pack.width = SPRITE_WIDTH;
    pack.height = SPRITE_HEIGHT;
    pack.rotation = ROTATION;
    pack.path = RES_PATH_PARTICLES_EXPLOSION;
    PrepareParticles(pack);
    MakeInvokeForDestroy(BEAUTIFUL_DESTROY_TIME);
}

ExplosionParticles::~ExplosionParticles() = default;

const size_t ExplosionParticles::kTypeId = TypeId_Trail_Explosion;