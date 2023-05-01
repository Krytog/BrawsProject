#define __SERVER_ENGINE_MODE__
#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ServerGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Game/Tools/Decoy/ServerDecoy.h>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    ServerGameManagement::InitGameServer();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        auto from_client = DecoyServer::Receive();
        ServerGameManagement::HandleInput(1, from_client);

        engine.Update();

        overmind.UpdateCelebratesInfo();
        auto data_for_client = overmind.GetCerebratesInfoSerialized();

        DecoyServer::Send(data_for_client);

    }
    return 0;
}
