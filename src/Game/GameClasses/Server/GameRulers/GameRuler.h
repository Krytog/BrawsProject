#pragma once

#include <Game/GameClasses/Server/Pawns/Characters/CharacterPawnServer.h>
#include <unordered_set>

class GameRuler {
public:
    static GameRuler& GetInstance();
    ~GameRuler();

    void AddPlayer(const CharacterPawnServer* player);
    void PlayerLoses(const CharacterPawnServer* player);
    void PlayerWins(const CharacterPawnServer* player);

    bool IsGameEnded() const;
    void BeginGame();

private:
    GameRuler();
    GameRuler(const GameRuler&) = delete;
    GameRuler& operator=(const GameRuler&) = delete;

    std::unordered_set<const CharacterPawnServer*> players_;
    bool has_begun_ = false;

    void MakeEventForStoppingGame() const;
};
