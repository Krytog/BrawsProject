#pragma once

#include "../CharacterPawnClient.h"

class CharacterDefaultPawnClient : public CharacterPawnClient {
public:
    CharacterDefaultPawnClient();
    CharacterDefaultPawnClient(const Position& position);

    void Shoot() override;
    void ReceiveDamage() override;

    void OnUpdate() override;

    static const size_t kTypeId;
};