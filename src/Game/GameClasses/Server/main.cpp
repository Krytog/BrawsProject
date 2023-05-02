#define __SERVER_ENGINE_MODE__
#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ServerGameManagement.h>
#include <SwarmSystem/Overmind.h>

#include <Infrastructure/Server/Communicator.h>

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    Communicator& communicator = Communicator::GetInstance();
    uint64_t id = communicator.RegUser();
    ServerGameManagement::InitGameServer();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        auto from_client = communicator.ReceiveFromClient(id);
        ServerGameManagement::HandleInput(1, from_client);

        engine.Update();

        overmind.UpdateCelebratesInfo();
        auto data_for_client = overmind.GetCerebratesInfoSerialized();

        communicator.SendToClient(id, data_for_client);
    }
    return 0;
}
