#include "Cursor.h"

#include <Core/ClientEngine.h>
#include <Game/Resources/ResourcesPaths.h>

enum {
    SPRITE_WIDTH = 128,
    SPRITE_HEIGHT = 128
};

Cursor::Cursor() : VisibleGameObject() {
    visible_object_ = std::make_unique<BasicSprite>(RES_PATH_UI_AIM, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, 0, IVisibleObject::RenderLayers::UI);
}

void Cursor::OnUpdate() {
    auto input = ClientEngine::GetInstance().GetInput();
    auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
    UpdatePosition(mouse_token.position);
}