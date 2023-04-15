#pragma once

#include "../../../../../SwarmSystem/Cerebrate.h"
#include "../../../../../Core/Engine.h"

class CharacterPawnClient;

class CharacterCerebrateClient : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        double current_health;
    };

    CharacterCerebrateClient(CharacterPawnClient* pawn_to_possess, bool is_controllable);
    virtual ~CharacterCerebrateClient();

    virtual void ForcePossessedExecuteCommand(const std::string& serialized_command) const override;
    virtual std::string SerializeInfo() const override;
    virtual void UsePossessedApi(std::string_view serialized_command) const;


protected:
    CharacterPawnClient* possessed_;
    bool is_controllable_;
};