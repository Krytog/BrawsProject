#include "PowerUpParticles.h"

#include <Game/Resources/ResourcesPaths.h>
#include <SwarmSystem/TypeIdList.h>

enum {
    SPRITE_WIDTH = 250,
    SPRITE_HEIGHT = 156,
    TICKS = 1,
    FRAMES = 39,
    CYCLED = false,
    ROTATION = 0
};

#define BEAUTIFUL_DESTROY_TIME 40
#define COEF 1.5

PowerUpParticles::PowerUpParticles() : PowerUpParticles(Position()) {}

PowerUpParticles::PowerUpParticles(const Position &position) {
    Particles::ArgPack pack;
    pack.pos = position;
    pack.ticks = TICKS;
    pack.frames = FRAMES;
    pack.width = SPRITE_WIDTH;
    pack.width *= COEF;
    pack.height = SPRITE_HEIGHT;
    pack.height *= COEF;
    pack.rotation = ROTATION;
    pack.path = RES_PATH_PARTICLES_POWERUP;
    PrepareParticles(pack);
    MakeInvokeForDestroy(BEAUTIFUL_DESTROY_TIME);
}

PowerUpParticles::~PowerUpParticles() = default;

const size_t PowerUpParticles::kTypeId = TypeId_Trail_PowerUp;