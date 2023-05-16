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
//    uint64_t player2 = communicator.RegUser();
    communicator.Run();
    players_id.push_back(player1);
//    players_id.push_back(player2);
    ServerGameManagement::InitGameServer(players_id);
    MyTime time;
    engine.SetActiveOn();

    Profiler& profiler = Profiler::GetInstance();

    while (engine.IsActive()) {
//        if (time.EvaluateTime() < 1.0 / 30) {
//            continue;
//        }
        time.ResetTime();

        for (auto player : players_id) {
            auto from_client = communicator.ReceiveFromClient(player);
//            std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - profiler.ExtractTimeMark(&from_client);
//            std::cout << "FROM CLIENT " << time_interval.count() * 1000 << "ms" << std::endl;
            ServerGameManagement::HandleInput(player, from_client);
        }

        engine.Update();

//         overmind.DebugInfo();
//         engine.DebugInfo();

        for (auto player : players_id) {
            ServerGameManagement::PrepareAndSendDataToClient(player);
        }
//        Profiler::GetInstance().PrintResults();

        std::this_thread::sleep_for (std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime()))));

    }
}

//int main() {
//    Communicator& communicator = Communicator::GetInstance();
//    Profiler& profiler = Profiler::GetInstance();
//    auto id = communicator.RegUser();
//    MyTime time;
//    const std::string data = "bebrabebrabebra";
//    while (true) {
//        if (time.EvaluateTime() < 1.0 / 60) {
//            continue;
//        }
//        time.ResetTime();
//        std::string from = communicator.ReceiveFromClient(id);
//        std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - profiler.ExtractTimeMark(&from);
//        std::cout << "FROM CLIENT " << time_interval.count() * 1000 << "ms" << std::endl;
//
//        auto package = data;
//        profiler.AddTimeMark(&package);
//        communicator.SendToClient(id, package);
//
//        communicator.RunFor(15);
//    }
//}
