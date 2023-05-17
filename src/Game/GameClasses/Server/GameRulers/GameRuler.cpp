#include "GameRuler.h"

#include <Game/GameClasses/CommandsList.h>
#include <Core/ServerEngine.h>

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

void GameRuler::AddPlayer(Cerebrate* player_cerebrate) {
    players_.insert(player_cerebrate);
}

void GameRuler::PlayerLoses(Cerebrate* player_cerebrate) {
    std::string command;
    command.push_back(CharacterCommands::COMMAND_ON_LOSE);
    player_cerebrate->AddCommandToBuffer(command);
    players_.erase(player_cerebrate);
}

void GameRuler::PlayerWins(Cerebrate *player_cerebrate) {
    std::string command;
    command.push_back(CharacterCommands::COMMAND_ON_WIN);
    player_cerebrate->AddCommandToBuffer(command);
    players_.erase(player_cerebrate);

    std::string lose_command;
    lose_command.push_back(CharacterCommands::COMMAND_ON_LOSE);
    while (!players_.empty()) {
        auto cerebrate = players_.begin();
        (*cerebrate)->AddCommandToBuffer(lose_command);
        players_.erase(cerebrate);
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