#include "GameRuler.h"

#include <Game/GameClasses/CommandsList.h>
#include <SwarmSystem/Overmind.h>

#define END_DELAY 0

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

GameRuler::GameRuler(): alive_players_(0) {
    MakeEventForStoppingGame();
}

bool GameRuler::IsGameEnded() const {
    return has_begun_ && alive_players_ <= 1;
}

void GameRuler::AddPlayer(const CharacterPawnServer* player) {
    if (!is_alive_.contains(player)) {
        is_alive_[player] = true;
        ++alive_players_;
    }

}

void GameRuler::PlayerLoses(const CharacterPawnServer* player) {
    if (is_alive_.contains(player) && is_alive_.at(player)) {
        is_alive_[player] = false;
        --alive_players_;
    }
}

void GameRuler::PlayerWins(const CharacterPawnServer* player) {
    if (!is_alive_.contains(player)) {
        return;
    }
    is_alive_[player] = true; // post mortem
    for (auto& pr : is_alive_) {
        if (pr.second && pr.first != player) {
            pr.second = false;
        }
    }
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
    ServerEngine::GetInstance().Invoke(std::chrono::milliseconds(END_DELAY), &ServerEngine::SetActiveOff, &ServerEngine::GetInstance());
}

bool GameRuler::IsPlayerAlive(const CharacterPawnServer* player) {
    return is_alive_[player];
}