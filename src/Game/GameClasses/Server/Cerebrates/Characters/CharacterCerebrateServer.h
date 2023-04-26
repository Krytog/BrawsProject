#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <Core/Engine.h>

class CharacterPawnServer;

class CharacterCerebrateServer : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        double current_health;
    };

    CharacterCerebrateServer(CharacterPawnServer* pawn_to_possess);
    virtual ~CharacterCerebrateServer();

    virtual void ForcePossessedExecuteCommand(const std::string& serialized_command) const override;
    virtual std::string SerializeInfo() override;
    virtual void UsePossessedApi(std::string_view serialized_command) const;

protected:
    CharacterPawnServer* possessed_;
};