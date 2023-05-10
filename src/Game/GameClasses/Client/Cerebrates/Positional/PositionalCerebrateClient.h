#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <Core/ClientEngine.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include <Game/GameClasses/CommandsList.h>

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, UpdatePosition, SetRotation)>
class PositionalCerebrateClient : public Cerebrate {
public:
    struct Info {
        Position position;
        Vector2D rotator;
    };

    PositionalCerebrateClient() : Cerebrate(TPawn::kTypeId) {
        auto ptr = ClientEngine::GetInstance().CreateGameObjectByDefault<TPawn>();
        possessed_ = dynamic_cast<TPawn*>(ptr);
    }

    virtual ~PositionalCerebrateClient() {
        ClientEngine::GetInstance().Destroy(possessed_);
    }

    void* GetPossessed() const override {
        return possessed_;
    }

    void ForcePossessedExecuteCommand(std::string_view serialized_command) const override {
        if (serialized_command.size() < sizeof(Info)) {
            return;
        }
        Info current_info;
        Serializer::Deserialize(current_info, serialized_command.substr(0, sizeof(Info)));
        possessed_->UpdatePosition(current_info.position);
        possessed_->SetRotation(current_info.rotator);
    }


    std::string SerializeInfo() override {
        return {};
    }

    void UsePossessedApi(std::string_view serialized_command) const {
        // no api
    }

protected:
    TPawn* possessed_;
};