#pragma once

#include "../CharacterPawnServer.h"

class CharacterDefaultPawnServer : public CharacterPawnServer {
public:
    CharacterDefaultPawnServer();
    CharacterDefaultPawnServer(const Position& position);
    CharacterDefaultPawnServer(const CharacterPawnServer::ArgPack& base_arg_pack, const Position& position);

    ~CharacterDefaultPawnServer() override;

    void Shoot(const Position& position) override;

    void OnUpdate() override;

    static const size_t kTypeId;
};