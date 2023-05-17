#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <Core/ClientEngine.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include <Game/GameClasses/CommandsList.h>

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, UpdatePosition, SetRotation,
                                                                CaptureViewPort, ReceiveDamage, Shoot,
                                                                SetAmmoLeft, SetCooldown, SetHealth)>
class CharacterCerebrateClient : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        Vector2D rotator;
        double current_health;
        double cooldown;
        int8_t ammo;
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

    void ForcePossessedExecuteCommand(std::string_view serialized_command) override {
        if (serialized_command.size() < sizeof(Info)) {
            return;
        }
        Info actual_info;
        auto info_size = sizeof(actual_info);
        Serializer::Deserialize(actual_info, serialized_command.substr(0, info_size));
        HandleCommands(serialized_command.substr(info_size));
        possessed_->UpdatePosition(actual_info.current_pos);
        if (!is_controlled_) {
            possessed_->SetRotation(actual_info.rotator);
        }
        possessed_->SetHealth(actual_info.current_health);
        possessed_->SetAmmoLeft(actual_info.ammo);
        possessed_->SetCooldown(actual_info.cooldown);
    }


    std::string SerializeInfo() override {
        return {};
    }

    void HandleCommands(std::string_view serialized_command) {
        for (auto command : serialized_command) {
            switch (command) {
                case CharacterCommands::COMMAND_CAPTURE_VIEWPORT: {
                    possessed_->CaptureViewPort();
                    is_controlled_ = true;
                    break;
                }
                case CharacterCommands::COMMAND_RECEIVE_DAMAGE: {
                    possessed_->ReceiveDamage();
                    break;
                }
                case CharacterCommands::COMMAND_SHOOT: {
                    possessed_->Shoot();
                    break;
                }
            }
        }
    }

    bool IsInManualMode() const override {
        return is_controlled_;
    }

protected:
    TPawn* possessed_;
    bool is_controlled_ = false;
};