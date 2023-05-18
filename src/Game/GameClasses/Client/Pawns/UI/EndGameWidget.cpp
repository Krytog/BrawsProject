#include "EndGameWidget.h"

#include <Game/Resources/ResourcesPaths.h>

#define DELAY_TIME 5000

enum {
    SPRITE_WIDTH = 1000,
    SPRITE_HEIGHT = 800
};

EndGameWidget::EndGameWidget(Status status) {
    ClientEngine::GetInstance().Invoke(std::chrono::milliseconds(DELAY_TIME), &ClientEngine::SetActiveOff, &ClientEngine::GetInstance());
    VisibleGameObject::UpdatePosition(ClientEngine::GetInstance().GetCameraPosition());
    switch (status) {
        case Status::WIN: {
            visible_object_ = std::make_unique<BasicSprite>(RES_PATH_UI_WIN, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, 0, IVisibleObject::RenderLayers::UI_extra);
            break;
        }
        case Status::LOSE: {
            visible_object_ = std::make_unique<BasicSprite>(RES_PATH_UI_LOSE, *position_, SPRITE_WIDTH, SPRITE_HEIGHT, 0, IVisibleObject::RenderLayers::UI_extra);
            break;
        }
    }
}