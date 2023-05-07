#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Infrastructure/Client/Communicator.h>
#include <BotAPI/BotInputSystem.h>
#include <BotAPI/BotManagement.h>
#include <SwarmSystem/Profiler/Profiler.h>

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    communicator.RegOnServer();
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();
    MyTime breakdown;
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        auto data = communicator.ReceiveFromServer();
        auto time_point = Profiler::GetInstance().ExtractTimeMark(&data);
        Profiler::GetInstance().HandlePing(time_point);
        std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - time_point;
        std::cout << time_interval.count() * 1000 << "ms" << std::endl;

        Profiler::GetInstance().StartSwarmSystemFrame();
        overmind.ForceCerebratesExecuteCommands(data);
        Profiler::GetInstance().FinishSwarmSystemFrame();

        Profiler::GetInstance().StartEngineFrame();
        engine.Update();
        Profiler::GetInstance().FinishEngineFrame();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);

        if (breakdown.EvaluateTime() > 60) {
            engine.SetActiveOff();
        }
    }
    return 0;
}
