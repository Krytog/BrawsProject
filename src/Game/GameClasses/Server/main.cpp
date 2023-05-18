#include <Core/ServerEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ServerGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <NormInfrastructure/Server/Communicator.h>

#include <iostream>

#define SLEEP_TIME std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime())))

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

    while (engine.IsActive()) {
        time.ResetTime();

        for (auto player : players_id) {
            auto from_client = communicator.ReceiveFromClient(player);
            ServerGameManagement::HandleInput(player, from_client);
        }

        engine.Update();

        for (auto player : players_id) {
            ServerGameManagement::PrepareAndSendDataToClient(player);
        }

        std::this_thread::sleep_for(SLEEP_TIME);
    }
    communicator.Stop();
    std::cout << "KINDA CORRECT OUT" << std::endl;
    return 0;
}

