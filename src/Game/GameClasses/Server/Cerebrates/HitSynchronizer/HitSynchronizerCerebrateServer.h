#pragma once

#include <Core/Position.h>
#include <SwarmSystem/Cerebrate.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include <stdexcept>

template <typename TPawn, HasMethods(TPawn, GetType, GetPosition)>
class HitSynchronizerCerebrateServer : public Cerebrate {
public:
    struct Info {
        Position position;
    };

    HitSynchronizerCerebrateServer(TPawn* pawn_to_possess) : Cerebrate(pawn_to_possess->GetType()), possessed_(pawn_to_possess) {}

    ~HitSynchronizerCerebrateServer() override = default;

    void* GetPossessed() const override {
        return possessed_;
    }

    void ForcePossessedExecuteCommand(std::string_view serialized_command) override {
        // these objects can't accept any input
        throw std::runtime_error("Hit cerebrate forces pawn to do something");
    }

    std::string SerializeInfo() override {
        Info info;
        info.position = possessed_->GetPosition();
        std::string output;
        Serializer::Serialize(info, &output);
        return output;
    }

protected:
    TPawn* possessed_;
};