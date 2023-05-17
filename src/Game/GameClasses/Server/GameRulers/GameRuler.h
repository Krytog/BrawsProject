#pragma once

#include <SwarmSystem/Overmind.h>
#include <unordered_set>

class GameRuler {
public:
    static GameRuler& GetInstance();
    ~GameRuler();

    void AddPlayer(Cerebrate* player_cerebrate);
    void PlayerLoses(Cerebrate* player_cerebrate);
    void PlayerWins(Cerebrate* player_cerebrate);

    bool IsGameEnded() const;
    void BeginGame();

private:
    GameRuler();
    GameRuler(const GameRuler&) = delete;
    GameRuler& operator=(const GameRuler&) = delete;

    std::unordered_set<Cerebrate*> players_;
    bool has_begun_ = false;

    void MakeEventForStoppingGame() const;
};
