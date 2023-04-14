#include "../Core/Engine.h"
#include "../Core/MyTime.h"
#include "../GameItself/Tools/GameManagementTools.h"

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    GameManagementTools::InitGame();
    MyTime time;
    engine.SetActiveOn();
    while (engine.IsActive()) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();
        engine.Update();
    }
    return 0;
}
