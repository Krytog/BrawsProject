#include <Core/ServerEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ServerGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <NormInfrastructure/Server/Communicator.h>

#include <iostream>

using namespace std::chrono_literals;

int main() {
    ServerEngine& engine = ServerEngine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    std::vector<uint64_t> players_id;
    uint64_t player1 = communicator.RegUser();
    communicator.Run();
    //uint64_t player2 = communicator.RegUser();
    players_id.push_back(player1);
    //players_id.push_back(player2);
    ServerGameManagement::InitGameServer(players_id);
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        time.ResetTime();

        for (auto player : players_id) {
            auto from_client = communicator.ReceiveFromClient(player);
            ServerGameManagement::HandleInput(player, from_client);
        }

        engine.Update();

//         overmind.DebugInfo();
//         engine.DebugInfo();

        for (auto player : players_id) {
            ServerGameManagement::PrepareAndSendDataToClient(player);
        }
//        Profiler::GetInstance().PrintResults();

        std::this_thread::sleep_for(15ms);
    }
    return 0;
}
