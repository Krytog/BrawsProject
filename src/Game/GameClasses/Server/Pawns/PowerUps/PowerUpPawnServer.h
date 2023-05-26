#pragma once

#include <Core/ServerEngine.h>
#include <Game/GameClasses/Server/Pawns/Characters/CharacterPawnServer.h>
#include <Game/GameClasses/Server/Pawns/Interfaces/IDamageable.h>

class PowerUpPawnServer : public GameObject, public IDamageable {
public:
    PowerUpPawnServer();
    virtual ~PowerUpPawnServer();

    void ReceiveDamage(double damage) override;

    void OnUpdate() override;

private:
    void CorrectSelfDestruction();
    virtual void ApplyEffect(CharacterPawnServer* character) = 0;
};
