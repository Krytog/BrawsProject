#pragma once

#include <Core/Engine.h>
#include "SwarmSystem/TypeIdList.h"

class CharacterPawnClient : public GameObject {
public:
    CharacterPawnClient();
    virtual ~CharacterPawnClient();
    virtual void OnUpdate();


    virtual void Shoot(const Position& position) = 0;
    virtual void ReceiveDamage() = 0;

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