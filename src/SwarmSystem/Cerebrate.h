#pragma once

#include <string>

class Cerebrate {
public:
    Cerebrate(size_t type);
    virtual void ForcePossessedExecuteCommand(std::string_view serialized_command) = 0;
    virtual void* GetPossessed() const = 0; // We can replicate different types
    std::string GetInfoForOvermind();
    virtual std::string SerializeInfo() = 0;
    void AddCommandToBuffer(std::string_view serialized_command);
    void MarkDeprecated();
    bool IsDeprecated() const;
    size_t GetType() const;

    virtual bool IsInManualMode() const; // If cerebrate is in manual mode, then ActualizeCerebrates and RemoveDeprecated won't affect it

    virtual ~Cerebrate();

private:
    size_t type_;
    std::string buffer_;
    bool is_deprecated_ = false;
};