#include <Core/ClientEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Infrastructure/Client/Communicator.h>
#include <BotAPI/BotInputSystem.h>
#include <BotAPI/BotManagement.h>

#include <iostream>

int main() {
    ClientEngine& engine = ClientEngine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    communicator.RegOnServer();
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
        std::cout << "in " << data.size() << " " << data << std::endl;
        overmind.ActualizeCerebrates(data);
        std::cout << "out" << std::endl;


        std::cout << "Engine in" << std::endl;
        engine.Update();
        std::cout << "Engine out" << std::endl;

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);
    }
    return 0;
}
