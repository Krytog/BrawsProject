#include <sys/wait.h>

#include <Core/ClientEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>
#include <SwarmSystem/Profiler/Profiler.h>
#include <BotAPI/BotInputSystem.h>

#include <NormInfrastructure/Client/Communicator.h>
#include <NormInfrastructure/Client/Agent.h>
#include <cstddef>

#define SLEEP_TIME std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime())))

enum ControlType {
    MANUAL, BOT
};

void Game(uint16_t communicator_port, uint64_t user_id, ControlType controls) {
    ClientEngine& engine = ClientEngine::GetInstance();
    if (controls == ControlType::BOT) {
        engine.SwitchInputSystem<BotInputSystem>();
    }
    Communicator& communicator = Communicator::GetInstance(communicator_port);
    communicator.SetId(user_id);
    communicator.RegOnServer();
    communicator.Run();
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        time.ResetTime();
        ClientGameManagement::ReceiveAndHandleFromServer(communicator);

        engine.Update();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);

        std::this_thread::sleep_for(SLEEP_TIME);
    }
    communicator.Stop();
}

//int main(int argc, char* argv[]) {
//    Agent& agent = Agent::GetInstance();
//    uint64_t id = agent.GetUserID();
//    std::cout << "my id " <<  id << std::endl;
//
//    if (argc == 1)  {  // creator
//        std::cout << "lobby id " << agent.CreateGame(Character::PIRATE) << std::endl;
//    } else {
//        uint64_t lobby = strtoull(argv[1], NULL, 10);
//        std::cout << lobby << std::endl;
//        agent.JoinGame(Character::TANK, lobby);
//    }
//
//    while (true) {
//        if (agent.ApproveGame()) {
//            std::cout << "Game Approved" << std::endl;
//            pid_t pid = fork();
//            if (pid) {
//                waitpid(pid, NULL, 0);
//            } else {
//                execlp("./Dup.o", "./Dup.o", "./bot.o", "./Client_inner", std::to_string(agent.GetPort()).data(), std::to_string(id).data(), "1", NULL);
//            }
//            //     return 0;
//            break;
//        }
//    }
////    while (true) {
////
////    }
//
////    agent.LeaveGame();
////    std::cout << "Lobby ID " << agent.CreateGame(Character::PIRATE) << std::endl;
//
//    // zaprosi
//    return 0;
//}

enum {
    PORT_ARG = 1, ID_ARG = 2, CONTROL_TYPE = 3
};

int main(int argc, char **argv) {
    uint16_t port = std::stoi(argv[PORT_ARG]);
    uint64_t id = strtoull(argv[ID_ARG], NULL, 10);
    int type = std::stoi(argv[CONTROL_TYPE]);
    ControlType control = ControlType::MANUAL;
    if (type == 1) {
        control = ControlType::BOT;
    }
    Game(port, id, control);
    return 0;
}