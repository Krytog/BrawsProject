#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <Core/ClientEngine.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include <Game/GameClasses/CommandsList.h>

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, UpdatePosition, SetHealth, CaptureViewPort)>
class CharacterCerebrateClient : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        double current_health;
    };

    CharacterCerebrateClient() : Cerebrate(TPawn::kTypeId) {
        auto ptr = ClientEngine::GetInstance().CreateGameObjectByDefault<TPawn>();
        possessed_ = dynamic_cast<TPawn*>(ptr);
    }

    virtual ~CharacterCerebrateClient() {
        ClientEngine::GetInstance().Destroy(possessed_);
    }

    void* GetPossessed() const override {
        return possessed_;
    }

    void ForcePossessedExecuteCommand(std::string_view serialized_command) const override {
        if (serialized_command.size() < sizeof(Info)) {
            return;
        }
        Info actual_info;
        auto info_size = sizeof(actual_info);
        Serializer::Deserialize(actual_info, serialized_command.substr(0, info_size));
        UsePossessedApi(serialized_command.substr(info_size));
        possessed_->UpdatePosition(actual_info.current_pos);
        possessed_->SetHealth(actual_info.current_health);
    }


    std::string SerializeInfo() override {
        return {};
    }

    void UsePossessedApi(std::string_view serialized_command) const {
        for (auto command : serialized_command) {
            switch (command) {
                case CharacterCommands::COMMAND_CAPTURE_VIEWPORT: {
                    possessed_->CaptureViewPort();
                    break;
                }
            }
        }
    }

protected:
    TPawn* possessed_;
};