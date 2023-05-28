#pragma once

#include "../CharacterPawnServer.h"

class CharacterBullyPawnServer : public CharacterPawnServer {
public:
    CharacterBullyPawnServer();
    CharacterBullyPawnServer(const Position& position);
    CharacterBullyPawnServer(const CharacterPawnServer::ArgPack& base_arg_pack, const Position& position);

    ~CharacterBullyPawnServer() override;

    void Shoot(const Position& position) override;

    void OnUpdate() override;

    static const size_t kTypeId;
};