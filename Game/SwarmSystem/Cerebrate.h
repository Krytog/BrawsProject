#pragma once

#include <string>

class Cerebrate {
public:
    Cerebrate(size_t type);
    virtual void ForcePossessedExecuteCommand(const std::string& serialized_command) = 0;
    std::string GetInfoForOvermind();
    virtual std::string SerializeInfo() = 0;
    void AddCommandToBuffer(const std::string& serialized_command);
    size_t GetType();

    virtual ~Cerebrate();

private:
    size_t type_;
    std::string buffer_;
};