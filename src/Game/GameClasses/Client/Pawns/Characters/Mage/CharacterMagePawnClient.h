#pragma once

#include "../CharacterPawnClient.h"

class CharacterMagePawnClient : public CharacterPawnClient {
public:
    CharacterMagePawnClient();
    CharacterMagePawnClient(const Position& position);

    void Shoot(const Position& position) override;
    void ReceiveDamage() override;

    void OnUpdate() override;

    static const size_t kTypeId;
};
