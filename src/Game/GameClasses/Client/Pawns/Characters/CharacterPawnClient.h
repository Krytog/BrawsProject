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
    int8_t GetAmmoLeft() const;
    double GetCooldown() const;

    void SetHealth(double health);
    void SetAmmoLeft(int8_t ammo);
    void SetCooldown(double cooldown);

protected:
    virtual void AddStatsVisualization() const;

protected:
    double health_;
    int8_t ammo_left_;
    double cooldown_;

    bool is_controlled_ = false;
};