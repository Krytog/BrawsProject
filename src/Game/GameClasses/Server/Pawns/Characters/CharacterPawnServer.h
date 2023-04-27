#pragma once

#include <Core/Engine.h>
#include <Game/GameClasses/Server/Pawns/Interfaces/IMovable.h>

class CharacterPawnServer : public IMovable {
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
    size_t cerebrate_id;

    double health_;
    double damage_;
    double speed_;
};