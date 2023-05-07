#pragma once

#include "../CharacterPawnServer.h"

class CharacterMagePawnServer : public CharacterPawnServer {
public:
    CharacterMagePawnServer();
    CharacterMagePawnServer(const Position& position);
    CharacterMagePawnServer(const CharacterPawnServer::ArgPack& base_arg_pack, const Position& position);

    ~CharacterMagePawnServer() override;

    void Shoot(const Position& position) override;

    void OnUpdate() override;

    static const size_t kTypeId;
};