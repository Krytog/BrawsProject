#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <Core/Engine.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, UpdatePosition, SetHealth)>
class CharacterCerebrateClient : public Cerebrate {
public:
    struct Info {
        Position current_pos;
        double current_health;
    };

    CharacterCerebrateClient() : Cerebrate(TPawn::kTypeId) {
        auto ptr = Engine::GetInstance().CreateGameObjectByDefault<TPawn>();
        possessed_ = dynamic_cast<TPawn*>(ptr);
    }

    virtual ~CharacterCerebrateClient() {
        Engine::GetInstance().Destroy(possessed_);
    }

    virtual void ForcePossessedExecuteCommand(std::string_view serialized_command) const override {
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


    virtual std::string SerializeInfo() override {
        return {};
    }

    virtual void UsePossessedApi(std::string_view serialized_command) const {} // TODO

protected:
    TPawn* possessed_;
};