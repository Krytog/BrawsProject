#include "InputSystem.h"

#include <SFML/Window.hpp>

InputSystem::InputSystem(const sf::Window& window) : window_(window) {
}

InputSystem& InputSystem::GetInstance(const sf::Window& window) {
    static InputSystem instance(window);
    return instance;
}

// Implementation defined, make sure that MouseToken is ALWAYS at list.begin()!
void InputSystem::ReadNewInput() {
    input_tokens_.clear();

    uint8_t mouse_key = 0;
    static bool pressed = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!pressed) {
            pressed = true;
            mouse_key = 1;
        }
    } else {
        pressed = false;
    }

    auto mouse_pos = sf::Mouse::getPosition(window_);
    auto corrected_y = window_.getSize().y - mouse_pos.y;
    input_tokens_.emplace_back(MouseToken{mouse_key, Position(mouse_pos.x, corrected_y)});

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        input_tokens_.emplace_back(KeyboardToken{'D'});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        input_tokens_.emplace_back(KeyboardToken{'A'});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        input_tokens_.emplace_back(KeyboardToken{'W'});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        input_tokens_.emplace_back(KeyboardToken{'S'});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        input_tokens_.emplace_back(KeyboardToken{'Q'});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        input_tokens_.emplace_back(KeyboardToken{'E'});
    }
}

InputSystem::InputTokensArray InputSystem::GetInput() const {
    return input_tokens_;
}