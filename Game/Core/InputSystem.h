#pragma once

#include "Position.h"

#include <variant>
#include <stdint.h>
#include <list>

class InputSystem {
public:
    struct KeyboardToken {
        char symbol;
    };

    struct MouseClickToken {
        uint8_t key;
        Position position;
    };

    using InputToken = std::variant<KeyboardToken, MouseClickToken>;
    using InputTokensArray = const std::list<InputToken>&;

    static InputSystem& GetInstance();

    void ReadNewInput();
    InputTokensArray GetInput() const;

private:
    InputSystem();
    std::list<InputToken> input_tokens_;
};
