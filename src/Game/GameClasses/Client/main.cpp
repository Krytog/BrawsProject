#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>
#include <Game/Tools/InputManagement.h>

#include <Game/Tools/Decoy/ServerDecoy.h>

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        auto data = DecoyServer::ReceiveFromServer();
        overmind.ForceCerebratesExecuteCommands(data);

        engine.Update();
    }
    return 0;
}
