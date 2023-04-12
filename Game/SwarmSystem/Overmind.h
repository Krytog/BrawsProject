#pragma once

#include <unordered_map>
#include <memory>

#include "Cerebrate.h"

class Overmind {
public:
    static Overmind& GetInstance();

    void UpdateCelebratesInfo();
    const std::string& GetCerebratesInfoSerialized();
    void ForceCerebratesExecuteCommands(const std::string& serialized_command);
    void RegisterNewCerebrate(Cerebrate* cerebrate);

private:
    Overmind();

    std::string cerebrates_info_serialized_;
    size_t current_id_;
    std::unordered_map<size_t, Cerebrate*> cerebrates_;

    enum { AVG_GETINFO_STRLEN = 80 };
};