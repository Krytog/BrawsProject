#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include "Game/GameClasses/Server/ParsingUtilities/ControllerTools.h"

enum {
    MINIMAL_INPUT_SIZE = sizeof(Position) + 1,
    SHOOT_KEY = 1,
};

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, Move, Shoot, GetHealth, GetPosition,
        SetDirection, BlockShooting, UnblockShooting)>
class CharacterCerebrateServer : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        double current_health;
    };

    CharacterCerebrateServer(TPawn* pawn_to_possess) : Cerebrate(TPawn::kTypeId), possessed_(pawn_to_possess) {}

    ~CharacterCerebrateServer() override = default;

    void* GetPossessed() const override {
        return possessed_;
    }

    void ForcePossessedExecuteCommand(std::string_view serialized_command) override {
        if (serialized_command.size() < MINIMAL_INPUT_SIZE) {
            return;
        }
        Position aim_pos;
        size_t pos_bytes = Serializer::Deserialize(aim_pos, serialized_command.substr(0, sizeof(aim_pos)));
        uint8_t mouse_key = (serialized_command[pos_bytes] - '0');
        possessed_->SetDirection(aim_pos);
        if (mouse_key == SHOOT_KEY) {
            possessed_->Shoot(aim_pos);
            possessed_->BlockShooting();
        } else {
            possessed_->UnblockShooting();
        }
        Vector2D input_vector = ControllerTools::ResultVector(serialized_command.substr(pos_bytes + 1, std::string_view::npos));
        possessed_->Move(input_vector);
    }

    std::string SerializeInfo() override {
        Info actual_info;
        actual_info.current_health = possessed_->GetHealth();
        actual_info.current_pos = possessed_->GetPosition();
        std::string output;
        Serializer::Serialize(actual_info, &output);
        return output;
    }

protected:
    TPawn* possessed_;
};