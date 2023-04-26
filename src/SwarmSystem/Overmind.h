#pragma once

#include <unordered_map>
#include <memory>
#include <Core/GameObject.h>

#include "Cerebrate.h"

class Overmind {
public:
    static Overmind& GetInstance();

    void UpdateCelebratesInfo();
    const std::string& GetCerebratesInfoSerialized();
    void ForceCerebratesExecuteCommands(const std::string& serialized_command);
    void RegisterNewCerebrate(Cerebrate* cerebrate);
    void ActualizeCerebrate(const std::string& serialized_command);
    void DestroyCerebrate(size_t cerebrate_id);

    template <typename TCerebrate, typename TPawn>
    void CreateCerebrateToPossess(TPawn* pawn_to_possess) {
        auto ptr = new TCerebrate(pawn_to_possess);
        RegisterNewCerebrate(ptr);
    }

private:
    Overmind();
    ~Overmind();

    std::string cerebrates_info_serialized_;
    size_t current_id_ = 0;
    std::unordered_map<size_t, Cerebrate*> cerebrates_;

    enum { AVG_GETINFO_STRLEN = 80 };
};