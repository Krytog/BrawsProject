#include <sys/wait.h>

#include <Core/ClientEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>
#include <SwarmSystem/Profiler/Profiler.h>

#include <NormInfrastructure/Client/Communicator.h>
#include <NormInfrastructure/Client/Agent.h>

#define SLEEP_TIME std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime())))

void Game() {
    ClientEngine& engine = ClientEngine::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    communicator.Run();
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();

    while (engine.IsActive()) {
        time.ResetTime();

        ClientGameManagement::ReceiveAndHandleFromServer();

        engine.Update();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);

        std::this_thread::sleep_for(SLEEP_TIME);
    }
    communicator.Stop();
}

int main(int argc, char* argv[]) {
    Agent& agent = Agent::GetInstance();
//    while (true) {
//
//    }
    uint64_t id = agent.GetUserID();

    std::cout << "Our ID " <<  id << std::endl;
    std::cout << "Lobby ID " << agent.CreateGame(Character::PIRATE) << std::endl;
    //    agent.LeaveGame();
//    std::cout << "Lobby ID " << agent.CreateGame(Character::PIRATE) << std::endl;

    // zaprosi

//    if (!fork()) {
//        Game();
//    }
//    wait(NULL);
    return 0;
}