#include "Core/Engine.h"
#include "Core/MyTime.h"
#include "Game/Tools/GameManagementTools.h"
#include "SwarmSystem/Overmind.h"

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    GameManagementTools::InitGameClient();
    MyTime time;
    MyTime timer;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        overmind.UpdateCelebratesInfo();

        auto str = overmind.GetCerebratesInfoSerialized();
        std::cout << str << std::endl;
        engine.Update();

        overmind.ForceCerebratesExecuteCommands(str);

        if (timer.EvaluateTime() > 10) {
            engine.SetActiveOff();
        }
    }
    return 0;
}
