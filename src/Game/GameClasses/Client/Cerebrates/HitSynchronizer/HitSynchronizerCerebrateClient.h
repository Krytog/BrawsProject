#pragma once

#include <SwarmSystem/Cerebrate.h>
#include <Core/ClientEngine.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include <Game/GameClasses/CommandsList.h>

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, UpdatePosition)>
class HitSynchronizerCerebrateClient : public Cerebrate {
public:
    struct Info {
        Position position;
    };

    HitSynchronizerCerebrateClient() : Cerebrate(TPawn::kTypeId) {
        auto ptr = ClientEngine::GetInstance().CreateGameObjectByDefault<TPawn>();
        possessed_ = dynamic_cast<TPawn*>(ptr);
    }

    virtual ~HitSynchronizerCerebrateClient() = default; // pawns should take care of their deletions on their own

    void* GetPossessed() const override {
        return possessed_;
    }

    void ForcePossessedExecuteCommand(std::string_view serialized_command) override {
        if (serialized_command.size() < sizeof(Info)) {
            return;
        }
        Info current_info;
        Serializer::Deserialize(current_info, serialized_command.substr(0, sizeof(Info)));
        possessed_->UpdatePosition(current_info.position);
    }


    std::string SerializeInfo() override {
        return {};
    }

protected:
    TPawn* possessed_;
};