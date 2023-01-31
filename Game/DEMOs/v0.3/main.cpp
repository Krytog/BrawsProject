#include "../../../Game/Core/Sprites.h"
#include "../../../Game/Core/GameObject.h"
#include "../../../Game/Core/Render.h"
#include "../../../Game/Core/MyTime.h"
#include "../../../Game/Core/CollisionSystem.h"
#include "../../../Game/Core/Colliders.h"
#include "../../../Game/Core/Engine.h"


#include "Zelensky.h"
#include "Cursor.h"
#include "KripProducer.h"

#include <iostream>

#define RUN_LEFT "run_left"
#define RUN_RIGHT "run_right"
#define DEATH "death"

int main() {
    Engine& engine = Engine::GetInstance();

    auto aim = engine.ProduceObject<Cursor>(new Position(0, 0),
                                                new CircleCollider(Position(0, 0), 40, true),
                                                new StaticSprite(new Position(0, 0), 60, 50, "../Game/DEMOs/v0.3/Resources/aim.png", LEVELS::SECOND_USER_LEVEL),
                                                "aim");
    engine.ProduceObject<GameObject>(new Position(0, 0), nullptr, new StaticSprite(new Position(0, 0), 2000, 1000, "../Game/DEMOs/v0.3/Resources/background.jpg", LEVELS::BACKGROUND_LEVEL), "background");

    auto player = engine.ProduceObject<Zelensky>(new Position(0, 0),
                                                 new RectangleCollider(Position(0, 0), 60, 200),
                                                 nullptr,
                                                 "Zelensky_player",
                                                 aim);

    engine.ProduceObject<KripProducer>(nullptr, nullptr, nullptr, "kripoc", player, GameMode::GOD);

    engine.SetCameraOn(player);



    engine.ProduceObject<KripProducer>(new Position(0, 0), nullptr, nullptr, "producer", player, GameMode::HARD);

    MyTime time;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();
        engine.Update();
        //std::cout << time.EvaluateTime() << std::endl;
    }
    return 0;
}
