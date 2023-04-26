#pragma once

#include <unordered_map>
#include <memory>
#include <Core/GameObject.h>

#include "Cerebrate.h"

class Overmind {
public:
    static Overmind& GetInstance();

    void UpdateCelebratesInfo();
    void UpdateCelebratesInfo(Cerebrate* target, bool functor(Cerebrate*, Cerebrate*)) {} // TODO
    const std::string& GetCerebratesInfoSerialized();
    void ForceCerebratesExecuteCommands(const std::string& serialized_command);
    size_t RegisterNewCerebrate(Cerebrate* cerebrate);
    void ActualizeCerebrate(const std::string& serialized_command);
    void DestroyCerebrate(size_t cerebrate_id);

    template <typename TCerebrate, typename TPawn>
    size_t CreateCerebrateToPossess(TPawn* pawn_to_possess) {
        auto ptr = new TCerebrate(pawn_to_possess);
        return RegisterNewCerebrate(ptr);
    }

    void RegisterNewPlayer(uint64_t id, Cerebrate* cerebrate);
    Cerebrate* GetPlayersCerebrate(uint64_t id);
    void DeletePlayerFromRegistry(uint64_t id);

private:
    Overmind();
    ~Overmind();

    std::string cerebrates_info_serialized_;
    size_t current_id_ = 0;
    std::unordered_map<size_t, Cerebrate*> cerebrates_;
    std::unordered_map<uint64_t , Cerebrate*> players_cerebrate_;

    enum { AVG_GETINFO_STRLEN = 80 };
};