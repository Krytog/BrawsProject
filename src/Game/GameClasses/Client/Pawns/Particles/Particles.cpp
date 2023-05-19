#include "Particles.h"

Particles::Particles() = default;

Particles::Particles(const Particles::ArgPack &pack) : Particles() {
    PrepareParticles(pack);
}

void Particles::PrepareParticles(const Particles::ArgPack &pack) {
    visible_object_ = std::make_unique<AnimatedSprite>(pack.path, Position(),
                                                       pack.width, pack.height, pack.ticks, pack.frames,
                                                       false, 0, IVisibleObject::RenderLayers::Effects);
    VisibleGameObject::UpdatePosition(pack.pos);
}

void Particles::MakeInvokeForDestroy(size_t delay_ticks) {
    ClientEngine::GetInstance().Invoke(delay_ticks, &ClientEngine::Destroy, &ClientEngine::GetInstance(), this);
}

Particles::~Particles() = default;