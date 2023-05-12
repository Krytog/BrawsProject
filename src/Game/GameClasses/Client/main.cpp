#include <Core/ClientEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Infrastructure/Client/Communicator.h>

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
        overmind.ActualizeCerebrates(data);
        overmind.ForceCerebratesExecuteCommands(data);

        engine.Update();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);
    }
    return 0;
}
