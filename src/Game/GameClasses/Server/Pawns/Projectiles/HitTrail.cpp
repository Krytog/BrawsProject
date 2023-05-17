#include "HitTrail.h"

#include <Core/ServerEngine.h>
#include <SwarmSystem/Overmind.h>
#include <Game/GameClasses/Server/Cerebrates/HitSynchronizer/HitSynchronizerCerebrateServer.h>

#define COLLIDER_RADIUS 10

HitTrail::HitTrail(size_t type, uint64_t expire_ticks, const Position& pos): type_(type) {
    *position_ = pos;
    collider_ = std::make_unique<CircleCollider>(pos, COLLIDER_RADIUS, Collider::Category::Trigger);
    DelayedSelfDestruction(expire_ticks);
    cerebrate_id_ = Overmind::GetInstance().CreateCerebrateToPossess<HitSynchronizerCerebrateServer<HitTrail>>(this);
}

size_t HitTrail::GetType() const {
    return type_;
}

void HitTrail::DelayedSelfDestruction(uint64_t ticks_to_go) {
    ServerEngine::GetInstance().Invoke(ticks_to_go, &ServerEngine::Destroy, &ServerEngine::GetInstance(), this);
}

void HitTrail::OnUpdate() {}

HitTrail::~HitTrail() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id_);
}