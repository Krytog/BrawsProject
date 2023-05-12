#pragma once

#include <Core/Position.h>
#include <SwarmSystem/Cerebrate.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include <stdexcept>

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, GetPosition, GetRotator)>
class PositionalCerebrateServer : public Cerebrate {
public:
    struct Info {
        Position position;
        Vector2D rotator;
    };

    PositionalCerebrateServer(TPawn* pawn_to_possess) : Cerebrate(TPawn::kTypeId), possessed_(pawn_to_possess) {}

    ~PositionalCerebrateServer() override = default;

    void* GetPossessed() const override {
        return possessed_;
    }

    void ForcePossessedExecuteCommand(std::string_view serialized_command) override {
        // these objects can't accept any input
        throw std::runtime_error("Projectiles cerebrate forces pawn to do something");
    }

    std::string SerializeInfo() override {
        Info info;
        info.position = possessed_->GetPosition();
        info.rotator = possessed_->GetRotator();
        std::string output;
        Serializer::Serialize(info, &output);
        return output;
    }

protected:
    TPawn* possessed_;
};