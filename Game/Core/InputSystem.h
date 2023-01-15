#pragma once

#include "Position.h"

#include <variant>
#include <stdint.h>
#include <list>

struct KeyboardToken {
    char symbol;
};

struct MouseClickToken {
    uint8_t key;
    Position position;
};

class InputSystem {
public:
    using InputToken = std::variant<KeyboardToken, MouseClickToken>;

    static InputSystem& GetInstance();

    void ReadNewInput();
    const std::list<InputToken>& GetInput() const;

private:
    InputSystem();
    std::list<InputToken> input_tokens_;
};
