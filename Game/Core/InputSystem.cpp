#include "InputSystem.h"

InputSystem::InputSystem() = default;

InputSystem &InputSystem::GetInstance() {
    static InputSystem instance;
    return instance;
}

void InputSystem::ReadNewInput() {
    //implementation defined;
}

const &InputSystem::GetInput() const {
    return input_tokens_;
}
