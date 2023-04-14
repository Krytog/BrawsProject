#pragma once

#include <string>

class Cerebrate {
public:
    Cerebrate(size_t type);
    virtual void ForcePossessedExecuteCommand(const std::string& serialized_command) const = 0;
    std::string GetInfoForOvermind();
    virtual std::string SerializeInfo() const = 0;
    void AddCommandToBuffer(const std::string& serialized_command);
    size_t GetType() const;

    virtual ~Cerebrate();

private:
    size_t type_;
    std::string buffer_;
};