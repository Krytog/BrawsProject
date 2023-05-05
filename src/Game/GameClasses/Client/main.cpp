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
    Profiler& profiler = Profiler::GetInstance();
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
        {
            auto time_mark = profiler.ExtractTimeMark(&data);
            profiler.HandlePing(time_mark);
        }

        profiler.StartSwarmSystemFrame();
        overmind.ForceCerebratesExecuteCommands(data);
        profiler.FinishSwarmSystemFrame();

        profiler.StartEngineFrame();
        engine.Update();
        profiler.FinishEngineFrame();

        auto for_server = ClientGameManagement::SerializeInput();
        communicator.SendToServer(for_server);

        if (breakdown.EvaluateTime() > 60) {
            engine.SetActiveOff();
        }
    }
    return 0;
}
