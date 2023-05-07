#define __SERVER_ENGINE_MODE__
#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ServerGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Infrastructure/Server/Communicator.h>
#include <SwarmSystem/Profiler/Profiler.h>

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    std::vector<uint64_t> players_id;
    uint64_t player1 = communicator.RegUser();
    uint64_t player2 = 0;
    while (!player2) {
        player2 = communicator.RegUser();
    }
    players_id.push_back(player1);
    players_id.push_back(player2);
    ServerGameManagement::InitGameServer(players_id);
    MyTime time;
    engine.SetActiveOn();
    MyTime breakdown;
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        for (auto player : players_id) {
            auto from_client = communicator.ReceiveFromClient(player);
            ServerGameManagement::HandleInput(player, from_client);
        }

        Profiler::GetInstance().StartEngineFrame();
        engine.Update();
        Profiler::GetInstance().FinishEngineFrame();

        Profiler::GetInstance().StartSwarmSystemFrame();
        for (auto player : players_id) {
            ServerGameManagement::PrepareAndSendDataToClient(player);
        }
        Profiler::GetInstance().FinishSwarmSystemFrame();

        if (breakdown.EvaluateTime() > 90) {
            engine.SetActiveOff();
        }
    }
    return 0;
}
