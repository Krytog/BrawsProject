#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Infrastructure/Client/Communicator.h>

#include <iostream>
/*
int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    std::cout << "hello" << std::endl;
    communicator.RegOnServer();
    std::cout << "bye" << std::endl;
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        auto data = communicator.ReceiveFromServer();
        overmind.ForceCerebratesExecuteCommands(data);

        std::cout << "Hello: " << data.size() << " " << data << std::endl;

        engine.Update();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);
    }
    return 0;
}
*/

int main() {
    Communicator& communicator = Communicator::GetInstance();

    std::cout << "hello" << std::endl;
    uint64_t id = communicator.RegOnServer();
    std::cout << "bye" << std::endl;

    std::string str = "hello world";

    communicator.SendToServer(str);

    return 0;
}