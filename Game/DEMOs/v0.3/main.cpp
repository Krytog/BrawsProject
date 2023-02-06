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

int main() {
    Engine& engine = Engine::GetInstance();

    auto aim = engine.ProduceObject<Cursor>(new Position(0, 0),
                                                new CircleCollider(Position(0, 0), 25, true),
                                                nullptr,
                                                "aim");



    {
        VisibleObject* cursor_casual = new StaticSprite(new Position(0, 0), 50, 50, "../Game/DEMOs/v0.3/Resources/CursorCasual.png", LEVELS::SECOND_USER_LEVEL);
        VisibleObject* cursor_over = new StaticSprite(new Position(0, 0), 50, 50, "../Game/DEMOs/v0.3/Resources/CursorOver.png", LEVELS::SECOND_USER_LEVEL);

        engine.RenderSwith(aim, cursor_casual);

        auto menu_bg = engine.ProduceObject<GameObject>(new Position(0, 0),
                                                        nullptr,
                                                        new StaticSprite(new Position(0, 0), 1200, 760, "../Game/DEMOs/v0.3/Resources/background_menu.png", LEVELS::BACKGROUND_LEVEL), "background");

        VisibleObject* button_no_casual = new StaticSprite(new Position(-300, -240), 524, 141, "../Game/DEMOs/v0.3/Resources/NoCasual.png", LEVELS::FIRST_USER_LEVEL);
        VisibleObject* button_no_over = new StaticSprite(new Position(-300, -240), 524, 141, "../Game/DEMOs/v0.3/Resources/NoOver.png", LEVELS::FIRST_USER_LEVEL);

        auto button_no = engine.ProduceObject<GameObject>(new Position(-300, -240),
                                                           new RectangleCollider(Position(-300, -240), 524, 141, true),
                                                          nullptr, "no");

        VisibleObject* button_yes_casual = new StaticSprite(new Position(300, -240), 524, 141, "../Game/DEMOs/v0.3/Resources/YesCasual.png", LEVELS::FIRST_USER_LEVEL);
        VisibleObject* button_yes_over = new StaticSprite(new Position(300, -240), 524, 141, "../Game/DEMOs/v0.3/Resources/YesOver.png", LEVELS::FIRST_USER_LEVEL);

        auto button_yes = engine.ProduceObject<GameObject>(new Position(300, -240),
                                                          new RectangleCollider(Position(300, -240), 524, 141, true),
                                                          nullptr, "yes");


        const double i_wanna_die = 300;
        auto putin = engine.ProduceObject<GameObject>(new Position(-500 - i_wanna_die, -300 - i_wanna_die),
                                                      new CircleCollider(Position(-500 - i_wanna_die, -300 - i_wanna_die), 40),
                                                      new StaticSprite(new Position(-500 - i_wanna_die, -300 - i_wanna_die), 400, 500, "../Game/DEMOs/v0.3/Resources/Putin.png", LEVELS::SECOND_USER_LEVEL),
                                                      "putin");


        auto putin_stop1 = engine.ProduceObject<GameObject>(new Position(-300, -240),
                                                            new CircleCollider(Position(-300, -240), 100),
                                                            nullptr,
                                                            "stop1");

        auto putin_stop2 = engine.ProduceObject<GameObject>(new Position(-500- 1.3 * i_wanna_die, -300 - 1.3 * i_wanna_die),
                                                            new CircleCollider(Position(-500- 1.3 * i_wanna_die, -300 - 1.3 * i_wanna_die), 100),
                                                            nullptr,
                                                            "stop2");

        MyTime time;
        int putin_state = 0;
        while (true) {
            if (time.EvaluateTime() < static_cast<double>(1) / 60) {
                continue;
            }

            bool button_yes_switch_back = true;
            bool button_no_switch_back = true;

            int cursor_state = 0;

            {
                auto triggers = engine.GetTriggerCollisions(aim);
                for (auto& trigger : triggers) {
                    if (trigger.tag == "no") {
                        engine.RenderSwith(const_cast<GameObject *>(trigger.game_object), button_no_over);
                        button_no_switch_back = false;
                        cursor_state = 1;
                    } else if (trigger.tag == "yes") {
                        engine.RenderSwith(const_cast<GameObject *>(trigger.game_object), button_yes_over);
                        cursor_state = 2;
                        button_yes_switch_back = false;
                        if (putin_state == 0) {
                            putin_state = 1;
                        }
                    }
                }
            }

            if (button_no_switch_back) {
                engine.RenderSwith(button_no, button_no_casual);
            }
            if (button_yes_switch_back) {
                engine.RenderSwith(button_yes, button_yes_casual);
            }

            cursor_casual->UpdatePosition(aim->GetPosition());
            cursor_over->UpdatePosition(aim->GetPosition());
            if (button_yes_switch_back && button_no_switch_back) {
                engine.RenderSwith(aim, cursor_casual);
            } else {
                engine.RenderSwith(aim, cursor_over);
            }

            engine.ReadNewInput();
            auto input = engine.GetInput();
            auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
            if (mouse_token.key) {
                if (cursor_state == 1) {
                    std::cout << "CUT MY VEINS INTO PIECES" << std::endl;
                    engine.Destroy(menu_bg);
                    engine.Destroy(aim);
                    engine.Destroy(button_yes);
                    engine.Destroy(button_no);
                    engine.Destroy(putin);
                    engine.Destroy(putin_stop1);
                    engine.Destroy(putin_stop2);
                    delete button_yes_casual;
                    delete button_no_casual;
                    delete button_yes_over;
                    delete button_no_over;
                    delete cursor_casual;
                    delete cursor_over;
                    break;
                } else if (cursor_state == 2) {
                    return 0;
                }
            }

            if (auto kringe = engine.CheckPhysicalCollision(putin, putin_stop1)) {
                putin_state = 2;
            }
            if (auto kringe = engine.CheckPhysicalCollision(putin, putin_stop2)) {
                if (putin_state == 2) {
                    putin_state = 0;
                }
            }

            Vector2D cringe = Vector2D::Right + Vector2D::Up;
            cringe.Normalize();
            const double speed = 5;
            if (putin_state == 1) {
                putin->Translate(cringe * 5);
            } else if (putin_state == 2) {
                putin->Translate(cringe * -speed);
            }


            time.ResetTime();
            engine.Update();
        }
    }



    // Game phase

    aim = engine.ProduceObject<Cursor>(new Position(0, 0),
                                       new CircleCollider(Position(0, 0), 40, true),
                                       new StaticSprite(new Position(0, 0), 60, 50, "../Game/DEMOs/v0.3/Resources/aim.png", LEVELS::SECOND_USER_LEVEL),
                                       "aim");


    engine.ProduceObject<GameObject>(new Position(0, 0),
                                     nullptr,
                                     new StaticSprite(new Position(0, 500), 3000, 3000, "../Game/DEMOs/v0.3/Resources/maidan.jpg", LEVELS::BACKGROUND_LEVEL), "background");

    auto player = engine.ProduceObject<Zelensky>(new Position(0, 0),
                                                 new RectangleCollider(Position(0, 0), 60, 200),
                                                 nullptr,
                                                 "Zelensky_player",
                                                 aim);

    //engine.ProduceObject<KripProducer>(nullptr, nullptr, nullptr, "kripoc", player, GameMode::GOD);
    engine.SetCameraOn(player);



    engine.ProduceObject<KripProducer>(new Position(0, 0), nullptr, nullptr, "producer", player, GameMode::HARD);

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
