#include "Sprites.h"
#include "GameObject.h"
#include "Render.h"
#include "MyTime.h"

int main()
{
    MyTime time;
    Render render;
    Position pos(10, 10);
    std::unique_ptr<VisibleObject> vis_ptr = std::make_unique<StaticSprite>(&pos, 100, 100, std::string_view("../Game/Core/test_pics/kvaas.png"));
    auto bebra = vis_ptr.get();
    auto pos_ptr = std::make_unique<Position>(pos);
    std::unique_ptr<Collider> collider_ptr;
    GameObject* game_obj;
    game_obj = new GameObject(pos_ptr, collider_ptr, vis_ptr, "ia ebal git");
    render.AddToRender(game_obj, bebra);
    time.ResetTime();

    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        bebra->Translate(Vector2D::Up * 1);
        render.RenderAll();
        time.ResetTime();
    }
    return 0;
}
