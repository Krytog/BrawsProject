#include <Core/ClientEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <NormInfrastructure/Client/Communicator.h>

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
        time.ResetTime();

        auto data = communicator.ReceiveFromServer();
        overmind.ActualizeCerebrates(data);
        overmind.ForceCerebratesExecuteCommands(data);

        engine.Update();

        // overmind.DebugInfo();
        // engine.DebugInfo();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);

        communicator.RunFor(1000 * (static_cast<double>(1) / 60 - time.EvaluateTime() - 0.00005));
    }
    return 0;
}
