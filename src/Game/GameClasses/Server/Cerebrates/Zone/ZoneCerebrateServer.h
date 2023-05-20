#pragma once

#include <Core/Position.h>
#include <SwarmSystem/Cerebrate.h>
#include <SwarmSystem/Serializer.h>
#include <Core/Tools/Concepts.h>
#include <stdexcept>

#include <iostream>

template <typename TPawn, HasMember(TPawn, kTypeId), HasMethods(TPawn, GetPosition, GetWidth, GetHeight)>
class ZoneCerebrateServer : public Cerebrate {
public:
    struct Info {
        Position position;
        double width;
        double height;
    };

    ZoneCerebrateServer(TPawn* pawn_to_possess) : Cerebrate(TPawn::kTypeId), possessed_(pawn_to_possess) {}

    ~ZoneCerebrateServer() override = default;

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
        info.width = possessed_->GetWidth();
        info.height = possessed_->GetHeight();
        std::string output;
        Serializer::Serialize(info, &output);
        return output;
    }

protected:
    TPawn* possessed_;
};