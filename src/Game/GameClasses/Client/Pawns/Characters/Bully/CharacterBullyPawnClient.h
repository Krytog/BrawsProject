#pragma once

#include "../CharacterPawnClient.h"

class CharacterBullyPawnClient : public CharacterPawnClient {
public:
    CharacterBullyPawnClient();
    CharacterBullyPawnClient(const Position& position);
    ~CharacterBullyPawnClient() override;

    void Shoot() override;
    void ReceiveDamage() override;

    void OnUpdate() override;

    static const size_t kTypeId;
};
