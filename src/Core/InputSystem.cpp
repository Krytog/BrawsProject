#include "InputSystem.h"
#include "Qt/MainWidget/MainWidget.h"
#include "Qt/MainWidget/Render.h"
#include <qcursor.h>
#include <qt5/QtCore/qnamespace.h>

InputSystem::~InputSystem() = default;

InputSystem::InputTokensArray InputSystem::GetInput() const {
    return input_tokens_;
}

/* Default InputSystem */

KeyboardInputSystem* KeyboardInputSystem::InitInstance() {
    static KeyboardInputSystem instance;
    return &instance;
}

//Implementation defined, make sure that MouseToken is ALWAYS at list.begin()!
void KeyboardInputSystem::ReadNewInput() {
    input_tokens_.clear();

    uint8_t mouse_key = 0;
    static bool pressed = false;
    if (Render::GetInstance().OnMousePressed()) {
        if (!pressed) {
            pressed = true;
            mouse_key = 1;
        }
    } else {
        pressed = false;
    }

    input_tokens_ = Render::GetInstance().GetKeyBoardInput();
    auto cursor_pos =  Render::GetInstance().mapFromGlobal(QCursor::pos());
    Position cursor_local_pos = {cursor_pos.rx() - (kWindowWidth / 2), (kWindowHeight / 2) - cursor_pos.ry()};
    cursor_local_pos.Translate(Render::GetInstance().GetCameraPosition().GetCoordinatesAsVector2D());
    input_tokens_.insert(input_tokens_.begin(), MouseToken{mouse_key, cursor_local_pos});
}