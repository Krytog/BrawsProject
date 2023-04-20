#pragma once

#include "../../../../../Core/Engine.h"
#include "../../../../../Core/CustomBehaviour.h"
#include "../Interfaces/IMovable.h"

class CharacterPawnServer : public CustomBehaviour, public IMovable {
public:
    CharacterPawnServer();
    virtual ~CharacterPawnServer();
    virtual void OnUpdate();

    virtual void Shoot(const Position& position) = 0;

    double GetHealth() const;
    double GetDamage() const;
    double GetSpeed() const;

    void SetHealth(double health);
    void SetDamage(double damage);
    void SetSpeed(double speed);

protected:
    double health_;
    double damage_;
    double speed_;
};