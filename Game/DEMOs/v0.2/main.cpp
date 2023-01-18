#include "../../../Game/Core/Sprites.h"
#include "../../../Game/Core/GameObject.h"
#include "../../../Game/Core/Render.h"
#include "../../../Game/Core/MyTime.h"
#include "../../../Game/Core/CollisionSystem.h"
#include "../../../Game/Core/Colliders.h"
#include "../../../Game/Core/Engine.h"


#include "Bullet.h"
#include <iostream>

Vector2D ResultVector(const InputSystem::InputTokensArray& input) {
    Vector2D output(0, 0);
    for (auto token : input) {
        auto keyboard_token = std::get<InputSystem::KeyboardToken>(token);
        if (keyboard_token.symbol == 'D') {
            output += Vector2D::Right;
        } else if (keyboard_token.symbol == 'A') {
            output += Vector2D::Left;
        } else if (keyboard_token.symbol == 'W') {
            output += Vector2D::Up;
        } else if (keyboard_token.symbol == 'S') {
            output += Vector2D::Down;
        }
    }
    output.Normalize();
    return output;
}

int main() {
    Engine& engine = Engine::GetInstance();

    auto aim = engine.ProduceObject<GameObject>(new Position(0, 0),
                                                new CircleCollider(Position(0, 0), 40, true),
                                                new StaticSprite(new Position(0, 0), 60, 50, "../Game/DEMOs/v0.2/aim.png"),
                                                "aim");

    auto player = engine.ProduceObject<GameObject>(new Position(0, 0),
                                                   new RectangleCollider(Position(0, 0), 80, 80),
                                                   new StaticSprite(new Position(0, 0), 80, 80, "../Game/DEMOs/v0.2/kvaas.png"),
                                                   "player");

    engine.ProduceObject<GameObject>(new Position(0, 0), nullptr, new StaticSprite(new Position(0, 0), 2000, 1000, "../Game/DEMOs/v0.2/background.jpg"), "background");

    engine.SetCameraOn(player);

    std::unordered_set<Bullet*> musorka;

    MyTime time;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        time.ResetTime();

        //reading and handling input
        const double SPEED = 10;
        {
            engine.ReadNewInput();
            auto input = engine.GetInput();
            auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
            input.erase(input.begin());

            //shot
            if (mouse_token.key) {
                Vector2D shot_direction = aim->GetPosition().GetCoordinatesAsVector2D() - player->GetPosition().GetCoordinatesAsVector2D();
                shot_direction.Normalize();
                std::cout << shot_direction.GetCoordinates().first << " " << shot_direction.GetCoordinates().second << std::endl;
                engine.ProduceObject<Bullet>(new Position(player->GetPosition()),
                                             nullptr,
                                             new StaticSprite(new Position(player->GetPosition()), 60, 40, "../Game/DEMOs/v0.2/fireball.png"),
                                             "Bullet",
                                             shot_direction,
                                             &musorka);
            }

            auto res_vector = ResultVector(input);
            player->Translate(res_vector * SPEED);

            aim->UpdatePosition(mouse_token.position);
        }


        for (auto ptr : musorka) {
            ptr->Fly();
        }

        engine.TryExecuteDelayedCallbacks();
        engine.RenderAll();
    }



    return 0;
}
