#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <Core/Engine.h>
#include <SwarmSystem/Serializer.h>

template <typename TPawn>
class CharacterCerebrateServer : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        double current_health;
    };

    CharacterCerebrateServer(TPawn* pawn_to_possess) : Cerebrate(TPawn::kTypeId), possessed_(pawn_to_possess) {}

    virtual ~CharacterCerebrateServer() = default;

    virtual void ForcePossessedExecuteCommand(std::string_view serialized_command) const override {
        if (!serialized_command.empty()) {
            throw std::runtime_error("Data from client force server pawn to execute command!");
        }
    }

    virtual std::string SerializeInfo() override {
        Info actual_info;
        actual_info.current_health = possessed_->GetHealth();
        actual_info.current_pos = possessed_->GetPosition();
        std::string output;
        Serializer::Serialize(actual_info, &output);
        return output;
    }

    virtual void UsePossessedApi(std::string_view serialized_command) const {} // TODO

protected:
    TPawn* possessed_;
};