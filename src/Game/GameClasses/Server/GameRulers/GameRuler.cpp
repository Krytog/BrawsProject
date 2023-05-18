#include "GameRuler.h"

#include <Game/GameClasses/CommandsList.h>
#include <SwarmSystem/Overmind.h>

#define END_DELAY 2000

namespace {
    bool AlwaysTrue() {
        return true;
    }

    void SendCommand(const CharacterPawnServer* player, char to_send) {
        std::string command;
        command.push_back(to_send);
        Overmind::GetInstance().GetCerebrateWithId(player->GetCerebrateId())->AddCommandToBuffer(command);

    }

    void SendCommandForTime(const CharacterPawnServer* player, char to_send, uint64_t milliseconds) {
        Event* event = ServerEngine::GetInstance().CreateEvent(AlwaysTrue, {}, SendCommand, std::make_tuple(player, to_send), EventStatus::Reusable);
        ServerEngine::GetInstance().Invoke(std::chrono::milliseconds(milliseconds), &ServerEngine::DestroyEvent, &ServerEngine::GetInstance(), event);
    }
}

GameRuler &GameRuler::GetInstance() {
    static GameRuler instance;
    return instance;
}

GameRuler::GameRuler() {
    MakeEventForStoppingGame();
}

bool GameRuler::IsGameEnded() const {
    return has_begun_ && players_.size() <= 1;
}

void GameRuler::AddPlayer(const CharacterPawnServer* player) {
    players_.insert(player);
}

void GameRuler::PlayerLoses(const CharacterPawnServer* player) {
    //SendCommandForTime(player, CharacterCommands::COMMAND_ON_LOSE, END_DELAY);
    players_.erase(player);
}

void GameRuler::PlayerWins(const CharacterPawnServer* player) {
    //SendCommandForTime(player, CharacterCommands::COMMAND_ON_WIN, END_DELAY);
    //players_.erase(player);
}

void GameRuler::MakeEventForStoppingGame() {
    ServerEngine::GetInstance().CreateEvent(&GameRuler::IsGameEnded, std::make_tuple(this),
                                            &GameRuler::EndGame, std::make_tuple(this),
                                            EventStatus::Disposable);
}

GameRuler::~GameRuler() = default;

void GameRuler::BeginGame() {
    has_begun_ = true;
}

void GameRuler::EndGame() {
    if (!players_.empty()) {
        auto winner = *players_.begin();
        PlayerWins(winner);
    }
    ServerEngine::GetInstance().Invoke(std::chrono::milliseconds(END_DELAY), &ServerEngine::SetActiveOff, &ServerEngine::GetInstance());
}

bool GameRuler::IsPlayerAlive(const CharacterPawnServer* player) {
    return players_.contains(player);
}