#pragma once

#include <Core/ClientEngine.h>
#include "SwarmSystem/TypeIdList.h"

class CharacterPawnClient : public VisibleGameObject {
public:
    CharacterPawnClient();
    virtual ~CharacterPawnClient();
    virtual void OnUpdate();


    virtual void Shoot(const Position& position) = 0;
    virtual void ReceiveDamage() = 0;

    void CaptureViewPort() const;

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