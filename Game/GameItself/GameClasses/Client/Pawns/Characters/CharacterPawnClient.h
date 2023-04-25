#pragma once

#include "../../../../../Core/Engine.h"
#include "../../../../../Core/CustomBehaviour.h"
#include "../../../../../SwarmSystem/TypeIdList.h"

class CharacterPawnClient : public CustomBehaviour, public GameObject {
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

    size_t GetTypeId() const;

protected:
    size_t type_id_ = TypeId_UNDEFINED;

    double health_;
    double damage_;
    double speed_;
};