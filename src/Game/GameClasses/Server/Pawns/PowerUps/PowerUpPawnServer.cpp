#include "PowerUpPawnServer.h"

PowerUpPawnServer::PowerUpPawnServer() = default;
PowerUpPawnServer::~PowerUpPawnServer() = default;

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