#pragma once

#include "../CharacterPawnServer.h"

class DefaultCharacterPawnServer : public CharacterPawnServer {
public:
    DefaultCharacterPawnServer();
    DefaultCharacterPawnServer(const Position& position);

    void Shoot(const Position& position) override;

    void OnUpdate() override;
};