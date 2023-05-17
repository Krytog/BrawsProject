#include <Core/ServerEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ServerGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <NormInfrastructure/Server/Communicator.h>
#include <SwarmSystem/Profiler/Profiler.h>

#include <iostream>

using namespace std::chrono_literals;

int main() {
    ServerEngine& engine = ServerEngine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    std::vector<uint64_t> players_id;
    uint64_t player1 = communicator.RegUser();
    uint64_t player2 = communicator.RegUser();
    communicator.Run();
    players_id.push_back(player1);
    players_id.push_back(player2);
    ServerGameManagement::InitGameServer(players_id);
    MyTime time;
    engine.SetActiveOn();

    Profiler& profiler = Profiler::GetInstance();

    while (engine.IsActive()) {
        time.ResetTime();

        for (auto player : players_id) {
            auto from_client = communicator.ReceiveFromClient(player);
//            std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - profiler.ExtractTimeMark(&from_client);
//            std::cout << "FROM CLIENT " << time_interval.count() * 1000 << "ms" << std::endl;
            ServerGameManagement::HandleInput(player, from_client);
        }

        engine.Update();

        for (auto player : players_id) {
            ServerGameManagement::PrepareAndSendDataToClient(player);
        }

        std::this_thread::sleep_for (std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime()))));

    }
}

