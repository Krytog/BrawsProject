#pragma once

#include "../CharacterPawnServer.h"

class CharacterPiratePawnServer : public CharacterPawnServer {
public:
    CharacterPiratePawnServer();
    CharacterPiratePawnServer(const Position& position);
    CharacterPiratePawnServer(const CharacterPawnServer::ArgPack& base_arg_pack, const Position& position);

    ~CharacterPiratePawnServer() override;

    void Shoot(const Position& position) override;

    void OnUpdate() override;

    static const size_t kTypeId;
};