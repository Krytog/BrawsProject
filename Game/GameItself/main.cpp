#include "../Core/Engine.h"
#include "../Core/MyTime.h"
#include "../GameItself/Tools/GameManagementTools.h"
#include "../SwarmSystem/Overmind.h"

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    Overmind& overmind = Overmind::GetInstance();
    GameManagementTools::InitGame();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();


        overmind.UpdateCelebratesInfo();
        auto str = overmind.GetCerebratesInfoSerialized();
        engine.Update();


        overmind.ForceCerebratesExecuteCommands(str);
    }
    return 0;
}
