#pragma once

#include <Game/GameClasses/Server/Pawns/Characters/CharacterPawnServer.h>
#include <unordered_map>

class GameRuler {
public:
    static GameRuler& GetInstance();
    ~GameRuler();

    void AddPlayer(const CharacterPawnServer* player);
    void PlayerLoses(const CharacterPawnServer* player);
    void PlayerWins(const CharacterPawnServer* player);

    bool IsGameEnded() const;
    void BeginGame();

    void EndGame();

    bool IsPlayerAlive(const CharacterPawnServer* player);

private:
    GameRuler();
    GameRuler(const GameRuler&) = delete;
    GameRuler& operator=(const GameRuler&) = delete;

    std::unordered_map<const CharacterPawnServer*, bool> is_alive_;
    size_t alive_players_;
    bool has_begun_ = false;

    void MakeEventForStoppingGame();
};
