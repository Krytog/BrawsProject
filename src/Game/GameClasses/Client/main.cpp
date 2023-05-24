#include <sys/wait.h>

#include <Core/ClientEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>
#include <SwarmSystem/Profiler/Profiler.h>

#include <NormInfrastructure/Client/Communicator.h>
#include <NormInfrastructure/Client/Agent.h>
#include <cstddef>

#define SLEEP_TIME std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime())))

void Game(uint16_t communicator_port, uint64_t user_id) {
    std::cout << "Game Client" << std::endl;
    ClientEngine& engine = ClientEngine::GetInstance();
    std::cout << "safasf" << std::endl;
    Communicator& communicator = Communicator::GetInstance(communicator_port);
    std::cout << "dsfgsdgfdfg" << std::endl;
    communicator.SetId(user_id);
    // communicator.BindOnPort(communicator_port);
    std::cout << "before run" << std::endl;
    communicator.Run();
    std::cout << "after run" << std::endl;
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        time.ResetTime();
    // Overmind::GetInstance().DebugInfo();
        ClientGameManagement::ReceiveAndHandleFromServer(communicator);

        engine.Update();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);

        std::this_thread::sleep_for(SLEEP_TIME);
    }
    communicator.Stop();
}

int main(int argc, char* argv[]) {
    Agent& agent = Agent::GetInstance();
    uint64_t id = agent.GetUserID();
    std::cout << "my id " <<  id << std::endl;

    if (argc == 1)  {  // creator
        std::cout << "lobby id " << agent.CreateGame(Character::PIRATE) << std::endl;
    } else {
        uint64_t lobby = strtoull(argv[1], NULL, 10);
        std::cout << lobby << std::endl;
        agent.JoinGame(Character::MAGE, lobby);
    }

    while (true) {
        if (agent.ApproveGame()) {
            std::cout << "Game Approved" << std::endl;
                std::cout << "before Game" << std::endl;
                Game(agent.GetPort(), id);
            //     return 0;
            std::cout << "keeek" << std::endl;
            break;
        }
    }
//    while (true) {
//
//    }

//    agent.LeaveGame();
//    std::cout << "Lobby ID " << agent.CreateGame(Character::PIRATE) << std::endl;

    // zaprosi
    return 0;
}