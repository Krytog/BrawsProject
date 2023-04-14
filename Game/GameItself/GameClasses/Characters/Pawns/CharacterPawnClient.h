#pragma once

#include "../../../../Core/Engine.h"
#include "../../../../Core/CustomBehaviour.h"

class Character : public CustomBehaviour, public GameObject {
public:
    Character();
    virtual ~Character();

    virtual void Shoot(const Position& position) = 0;
    virtual void ReceiveDamage() = 0;

protected:
    double health_;
    double damage_;
    double speed_;
};