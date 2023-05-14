#pragma once

#include <Core/ClientEngine.h>
#include "SwarmSystem/TypeIdList.h"

class CharacterPawnClient : public VisibleGameObject {
public:
    CharacterPawnClient();
    virtual ~CharacterPawnClient();
    virtual void OnUpdate();


    virtual void Shoot() = 0;
    virtual void ReceiveDamage() = 0;

    void SetRotation(const Vector2D& rotator);

    void CaptureViewPort();

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

    bool is_controlled_;
};