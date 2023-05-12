#include "InputSystem.h"
#include "Position.h"
#include "Qt/MainWidget/MainWidget.h"
#include "Qt/MainWidget/Render.h"
#include <qcursor.h>

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
    auto& window = Render::GetInstance();
    input_tokens_ = window.GetKeyBoardInput();
    auto cursor_pos =  window.mapFromGlobal(QCursor::pos());
    Position cursor_local_pos = Position(cursor_pos.rx() - (kWindowWidth / 2), (kWindowHeight / 2) - cursor_pos.ry());
    cursor_local_pos.Translate(window.GetCameraPosition().GetCoordinatesAsVector2D());
    input_tokens_.push_front(MouseToken{Render::GetInstance().OnMousePressed(), cursor_local_pos});
}