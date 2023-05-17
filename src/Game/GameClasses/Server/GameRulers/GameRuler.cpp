#include "GameRuler.h"

#include <Game/GameClasses/CommandsList.h>
#include <SwarmSystem/Overmind.h>

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
    std::string command;
    command.push_back(CharacterCommands::COMMAND_ON_LOSE);
    Overmind::GetInstance().GetCerebrateWithId(player->GetCerebrateId())->AddCommandToBuffer(command);
    players_.erase(player);
}

void GameRuler::PlayerWins(const CharacterPawnServer* player) {
    std::string command;
    command.push_back(CharacterCommands::COMMAND_ON_WIN);
    Overmind::GetInstance().GetCerebrateWithId(player->GetCerebrateId())->AddCommandToBuffer(command);
    players_.erase(player);

    while (!players_.empty()) {
        auto player_to_lose = players_.begin();
        PlayerLoses(*player_to_lose);
    }
}

void GameRuler::MakeEventForStoppingGame() const {
    ServerEngine::GetInstance().CreateEvent(&GameRuler::IsGameEnded, std::make_tuple(this),
                                            &ServerEngine::SetActiveOff, std::make_tuple(&ServerEngine::GetInstance()),
                                            EventStatus::Disposable);
}

GameRuler::~GameRuler() = default;

void GameRuler::BeginGame() {
    has_begun_ = true;
}