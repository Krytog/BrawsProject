#pragma once

#include <unordered_map>
#include <memory>

#include "Cerebrate.h"

class Overmind {
public:
    static Overmind& GetInstance();

    void UpdateCerebratesInfo();
    void UpdateCerebratesInfo(Cerebrate* target, bool functor(Cerebrate*, Cerebrate*));
    const std::string& GetCerebratesInfoSerialized();
    void ForceCerebratesExecuteCommands(std::string_view serialized_command);
    size_t RegisterNewCerebrate(Cerebrate* cerebrate);
    void ActualizeCerebrates(std::string_view serialized_command);
    void DestroyCerebrate(size_t cerebrate_id);

    template <typename TCerebrate, typename TPawn>
    size_t CreateCerebrateToPossess(TPawn* pawn_to_possess) {
        auto ptr = new TCerebrate(pawn_to_possess);
        return RegisterNewCerebrate(ptr);
    }

    void RegisterNewPlayer(uint64_t id, size_t cerebrate_id);
    Cerebrate* GetPlayersCerebrate(uint64_t id);
    void DeletePlayerFromRegistry(uint64_t id);

    Cerebrate* GetCerebrateWithId(size_t id) const;

private:
    Overmind();
    Overmind(const Overmind&) = delete;
    Overmind& operator=(const Overmind&) = delete;
    ~Overmind();

    std::string cerebrates_info_serialized_;
    size_t current_id_ = 0;
    std::unordered_map<size_t, Cerebrate*> cerebrates_;
    std::unordered_map<uint64_t , size_t> players_cerebrate_;

    enum { AVG_GETINFO_STRLEN = 80 };
};