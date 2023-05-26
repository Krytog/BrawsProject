#include "PowerUpPawnServer.h"
#include <SwarmSystem/Overmind.h>

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
            CorrectSelfDestruction();
            break;
        }
    }
}

Vector2D PowerUpPawnServer::GetRotator() const {
    return Vector2D::Right;
}