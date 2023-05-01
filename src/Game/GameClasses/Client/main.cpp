#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Game/Tools/Decoy/ServerDecoy.h>

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    ClientGameManagement::InitGameClient();
    ClientGameManagement::InitRegistryForOvermind();
    MyTime time;
    engine.SetActiveOn();
    {
        engine.Update();
        auto for_server = ClientGameManagement::SerializeInput();
        DecoyServer::Send(for_server);
    }
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        auto data = DecoyServer::Receive();
        overmind.ForceCerebratesExecuteCommands(data);

        engine.Update();

        auto for_server = ClientGameManagement::SerializeInput();
        DecoyServer::Send(for_server);
    }
    return 0;
}
