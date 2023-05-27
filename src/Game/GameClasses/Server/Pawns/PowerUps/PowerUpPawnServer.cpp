#include "PowerUpPawnServer.h"
#include <SwarmSystem/Overmind.h>
#include <SwarmSystem/TypeIdList.h>
#include <Game/GameClasses/Server/Pawns/Projectiles/HitTrail.h>

PowerUpPawnServer::PowerUpPawnServer() = default;
PowerUpPawnServer::~PowerUpPawnServer() {
    Overmind::GetInstance().DestroyCerebrate(cerebrate_id_);
}

void PowerUpPawnServer::CorrectSelfDestruction() {
    ServerEngine::GetInstance().Invoke(0, &ServerEngine::Destroy, &ServerEngine::GetInstance(), this);
}

void PowerUpPawnServer::ReceiveDamage(double damage) {
    CorrectSelfDestruction();
}

void PowerUpPawnServer::OnUpdate() {
    auto collisions = ServerEngine::GetInstance().GetPhysicalCollisions(this);
    for (auto& collision : collisions) {
        if (auto character = dynamic_cast<CharacterPawnServer*>(collision.game_object)) {
            ApplyEffect(character);
            LeaveTrailWhenUsed();
            CorrectSelfDestruction();
            break;
        }
    }
}

Vector2D PowerUpPawnServer::GetRotator() const {
    return Vector2D::Right;
}

#define TRAIL_STAYS_TICKS 5

void PowerUpPawnServer::LeaveTrailWhenUsed() {
    ServerEngine::GetInstance().CreateGameObject<HitTrail>(TypeId_Trail_PowerUp, TRAIL_STAYS_TICKS, *position_);
}