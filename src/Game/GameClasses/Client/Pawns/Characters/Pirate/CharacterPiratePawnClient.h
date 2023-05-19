#pragma once

#include "../CharacterPawnClient.h"

class CharacterPiratePawnClient : public CharacterPawnClient {
public:
    CharacterPiratePawnClient();
    CharacterPiratePawnClient(const Position& position);
    ~CharacterPiratePawnClient() override;

    void Shoot() override;
    void ReceiveDamage() override;

    void OnUpdate() override;

    static const size_t kTypeId;
};
