#include <Core/ClientEngine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>
#include <SwarmSystem/Profiler/Profiler.h>

#include <NormInfrastructure/Client/Communicator.h>

#define SLEEP_TIME std::chrono::microseconds(int(1000000 * (1.0 / 60 - time.EvaluateTime())))

int main() {
    ClientEngine& engine = ClientEngine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    communicator.RegOnServer();
    communicator.Run();
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();

    while (engine.IsActive()) {
        time.ResetTime();

        auto data = communicator.ReceiveFromServer();

        if (data[0] == '#') { // means that we probably have a good package
            overmind.ActualizeCerebrates(data);
            overmind.ForceCerebratesExecuteCommands(data);
        }

        engine.Update();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);

        std::this_thread::sleep_for(SLEEP_TIME);
    }
    return 0;
}