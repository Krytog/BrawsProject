#pragma once

#include <Core/ServerEngine.h>
#include <Game/GameClasses/Server/Pawns/Characters/CharacterPawnServer.h>
#include <Game/GameClasses/Server/Pawns/Interfaces/IDamageable.h>

class PowerUpPawnServer : public GameObject, public IDamageable {
public:
    PowerUpPawnServer();
    virtual ~PowerUpPawnServer();

    void ReceiveDamage(double damage) override;
    Vector2D GetRotator() const;

    void OnUpdate() override;

protected:
    size_t cerebrate_id_;

    void CorrectSelfDestruction();
    virtual void ApplyEffect(CharacterPawnServer* character) = 0;
};
