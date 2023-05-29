#include <Core/ServerEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ServerGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <NormInfrastructure/Server/Porter.h>
#include <NormInfrastructure/Server/Communicator.h>

#include <iostream>

#define SLEEP_TIME std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime())))

void Game(const std::unordered_map<uint64_t, Player>& players) {
    ServerEngine& engine = ServerEngine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    std::vector<uint64_t> players_id = communicator.SetClients(players);
    communicator.RegAll();
    communicator.Run();

    ServerGameManagement::InitGameServer(players);

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
    engine.ClearAll();
}

int main() {
    Porter& porter = Porter::GetInstance();

    porter.StartRegistration();
    porter.StartHandling();
    while (true) {
        porter.CheckLobbiesState();
    }
}


