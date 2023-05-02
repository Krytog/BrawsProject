#define __SERVER_ENGINE_MODE__
#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ServerGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Infrastructure/Server/Communicator.h>

#include <iostream>

/*int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    std::cout << "hello" << std::endl;
    communicator.RegUser();
    std::cout << "bye" << std::endl;
    ServerGameManagement::InitGameServer();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        auto from_client = communicator.ReceiveFromClient(12);
        ServerGameManagement::HandleInput(1, from_client);

        engine.Update();

        overmind.UpdateCelebratesInfo();
        auto data_for_client = overmind.GetCerebratesInfoSerialized();

        communicator.SendToClient(12, data_for_client);
    }
    return 0;
}
 */

int main() {
    Communicator& communicator = Communicator::GetInstance();
    uint64_t id = communicator.RegUser();
    MyTime time;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 2) {
            continue;
        }
        time.ResetTime();

        auto str = communicator.ReceiveFromClient(id);
        std::cout << str.size() << str << std::endl;
    }
    return 0;
}
