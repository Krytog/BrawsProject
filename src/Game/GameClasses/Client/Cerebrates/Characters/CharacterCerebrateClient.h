#pragma once

#include "../../../../../SwarmSystem/Cerebrate.h"
#include "../../../../../Core/Engine.h"
#include "../../../../../SwarmSystem/Serializer.h"

template <typename TPawn>
class CharacterCerebrateClient : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        double current_health;
    };

    CharacterCerebrateClient(TPawn* pawn_to_possess, bool is_controllable)
    : possessed_(pawn_to_possess),
      is_controllable_(is_controllable),
      Cerebrate(TPawn::kTypeId) {}


    CharacterCerebrateClient() : CharacterCerebrateClient(nullptr, false) {
        auto ptr = Engine::GetInstance().CreateGameObjectByDefault<TPawn>();
        possessed_ = dynamic_cast<TPawn*>(ptr);
    }

    virtual ~CharacterCerebrateClient() = default;

    virtual void ForcePossessedExecuteCommand(const std::string& serialized_command) const override {
        Info actual_info;
        auto info_size = sizeof(actual_info);
        Serializer::Deserialize(actual_info, serialized_command.substr(0, info_size));
        UsePossessedApi(serialized_command.substr(info_size));
        possessed_->UpdatePosition(actual_info.current_pos);
        possessed_->SetHealth(actual_info.current_health);
    }


    virtual std::string SerializeInfo() override {
        if (is_controllable_) {
            HandleInput();
        }
        Info actual_info;
        actual_info.current_health = possessed_->GetHealth();
        actual_info.current_pos = possessed_->GetPosition();
        std::string output;
        Serializer::Serialize(actual_info, &output);
        return output;
    }

    virtual void UsePossessedApi(std::string_view serialized_command) const {}

    void HandleInput() { // TODO: make it independent of InputSystem impl
        auto input = Engine::GetInstance().GetInput();
        std::string commands;
        commands.reserve(4);
        for (auto& token : input) {
            auto keyboard_token = std::get_if<InputSystem::KeyboardToken>(&token);
            if (!keyboard_token) {
                continue;
            }
            if (keyboard_token->symbol == 'W') {
                commands += 'W';
            } else if (keyboard_token->symbol == 'A') {
                commands += 'A';
            } else if (keyboard_token->symbol == 'S') {
                commands += 'S';
            } else if (keyboard_token->symbol == 'D') {
                commands += 'D';
            }
        }
        AddCommandToBuffer(commands);
    }

protected:
    TPawn* possessed_;
    bool is_controllable_;
};