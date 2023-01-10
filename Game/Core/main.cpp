#include "Sprites.h"
#include "GameObject.h"
#include "Render.h"

int main()
{
    Render render;
    Position pos(10, 10);
    StaticSprite tree(&pos, 20, 20, std::string_view("../Game/Core/test_pics/kvaas.png"));
    const GameObject* game_obj;
    render.AddToRender(game_obj, &tree);
    while (true) {
        render.RenderAll();
    }
    return 0;
}
