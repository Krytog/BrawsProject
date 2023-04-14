#include "../Core/Engine.h"
#include "../Core/MyTime.h"
#include "../GameItself/Tools/GameManagementTools.h"

#include <iostream>

int main() {
    Engine& engine = Engine::GetInstance();
    GameManagementTools::InitGame();
    MyTime time;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        engine.Update();
    }
    return 0;
}
