#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <Core/Engine.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include <Game/Tools/ControllerTools.cpp>

enum {
    MINIMAL_INPUT_SIZE = sizeof(Position) + 1
};

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, Move, GetSpeed, GetHealth, GetPosition)>
class CharacterCerebrateServer : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        double current_health;
    };

    CharacterCerebrateServer(TPawn* pawn_to_possess) : Cerebrate(TPawn::kTypeId), possessed_(pawn_to_possess) {}

    virtual ~CharacterCerebrateServer() = default;

    virtual void ForcePossessedExecuteCommand(std::string_view serialized_command) const override {
        if (serialized_command.size() < MINIMAL_INPUT_SIZE) {
            return;
        }
        Position aim_pos;
        size_t pos_bytes = Serializer::Deserialize(aim_pos, serialized_command.substr(0, sizeof(aim_pos)));
        uint8_t mouse_key = serialized_command[pos_bytes];
        Vector2D input_vector = ControllerTools::ResultVector(serialized_command.substr(pos_bytes + 1, std::string_view::npos));
        possessed_->Move(input_vector * possessed_->GetSpeed());
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