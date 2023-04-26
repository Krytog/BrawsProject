#pragma once

#include "Position.h"

#include <variant>
#include <cstdint>
#include <list>
#include <SFML/Window.hpp>

class InputSystem {
public:
    struct KeyboardToken {
        char symbol;
    };

    struct MouseToken {
        uint8_t key;
        Position position;
    };

    using InputToken = std::variant<KeyboardToken, MouseToken>;
    using InputTokensArray = std::list<InputToken>;

    static InputSystem& GetInstance(const sf::Window& window);

    void ReadNewInput();
    InputTokensArray GetInput() const;

private:
    explicit InputSystem(const sf::Window& window);
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;

    InputTokensArray input_tokens_;
    const sf::Window& window_;
};
