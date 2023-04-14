#pragma once

#include "../CharacterPawnClient.h"

class DefaultCharacter : public Character {
public:
    DefaultCharacter();
    DefaultCharacter(const Position& position);

    void Shoot(const Position& position) override;
    void ReceiveDamage() override;
};