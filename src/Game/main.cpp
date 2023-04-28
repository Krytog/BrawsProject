#include <Core/Engine.h>
#include <Core/MyTime.h>
#include <Game/Tools/ClientGameManagement.h>
#include <SwarmSystem/Overmind.h>
#include <Game/Tools/InputManagement.h>

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    ClientGameManagement::InitGameClient();
    MyTime time;
    MyTime timer;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        overmind.UpdateCelebratesInfo();
        auto info = overmind.GetCerebratesInfoSerialized();

        engine.Update();

        overmind.ForceCerebratesExecuteCommands(info);

        auto str = InputManagement::SerializeInput();
        std::cout << str << std::endl;

        if (timer.EvaluateTime() > 10) {
            engine.SetActiveOff();
        }
    }
    return 0;
}
