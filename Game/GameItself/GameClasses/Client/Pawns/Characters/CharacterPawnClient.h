#pragma once

#include "../../../../../Core/Engine.h"
#include "../../../../../Core/CustomBehaviour.h"

class CharacterPawnClient : public CustomBehaviour, public GameObject {
public:
    CharacterPawnClient();
    virtual ~CharacterPawnClient();

    virtual void Shoot(const Position& position) = 0;
    virtual void ReceiveDamage() = 0;

protected:
    double health_;
    double damage_;
    double speed_;
};