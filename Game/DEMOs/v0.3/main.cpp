#include "../../../Game/Core/Sprites.h"
#include "../../../Game/Core/GameObject.h"
#include "../../../Game/Core/Render.h"
#include "../../../Game/Core/MyTime.h"
#include "../../../Game/Core/CollisionSystem.h"
#include "../../../Game/Core/Colliders.h"
#include "../../../Game/Core/Engine.h"


#include "Zelensky.h"
#include "Cursor.h"

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

    CharacterInitArgPack arg_pack;
    arg_pack.run_left_animation_path = "../Game/DEMOs/v0.3/Resources/ZelenskyRunsLeft.png";
    arg_pack.run_right_animation_path = "../Game/DEMOs/v0.3/Resources/ZelenskyRunsRight.png";
    arg_pack.death_animation_path = "../Game/DEMOs/v0.3/Resources/ZelenskyRunsRight.png";
    arg_pack.start_health = 100;
    arg_pack.shoot_pos = Position(0, 0);
    arg_pack.damage = 25;

    std::vector<std::pair<std::string_view, VisibleObject*>> decoy_vector;
    std::unordered_set<std::string_view> interrupt_points = {RUN_RIGHT, RUN_LEFT, DEATH};

    VisibleObject* run_left = new AnimatedSprite(new Position(0, 0), 200, 250, arg_pack.run_left_animation_path, LEVELS::FIRST_USER_LEVEL, 10, 18, 1, {0}, true);
    VisibleObject* run_right = new AnimatedSprite(new Position(0, 0), 200, 250, arg_pack.run_right_animation_path, LEVELS::FIRST_USER_LEVEL, 10, 18, 1, {0}, true);
    VisibleObject* death = new AnimatedSprite(new Position(0, 0), 200, 250, arg_pack.death_animation_path, LEVELS::FIRST_USER_LEVEL, 10, 18, 1, {0}, false);

    decoy_vector.emplace_back(RUN_LEFT, run_left);
    decoy_vector.emplace_back(RUN_RIGHT, run_right);
    decoy_vector.emplace_back(DEATH, death);

    auto player = engine.ProduceObject<Zelensky>(new Position(0, 0),
                                                 new RectangleCollider(Position(0, 0), 200, 250),
                                                 //run_left,
                                                 new AnimationSequencer(decoy_vector, interrupt_points, true),
                                                 "Zelensky_player",
                                                 arg_pack,
                                                 aim,
                                                 20);
    engine.SetCameraOn(player);


    std::cout << "HELLO" << std::endl;

    MyTime time;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();
        engine.Update();
    }
    return 0;
}
