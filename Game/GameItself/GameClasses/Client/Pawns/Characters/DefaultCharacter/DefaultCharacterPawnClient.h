#pragma once

#include "../CharacterPawnClient.h"

class DefaultCharacterPawnClient : public CharacterPawnClient {
public:
    DefaultCharacterPawnClient();
    DefaultCharacterPawnClient(const Position& position);

    void Shoot(const Position& position) override;
    void ReceiveDamage() override;

    void OnUpdate() override;
};