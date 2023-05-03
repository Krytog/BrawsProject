#pragma once

#include "Position.h"

#include <variant>
#include <cstdint>
#include <list>
#include <memory>
#include <SFML/Window.hpp>

class InputSystem;

template<typename T, std::enable_if_t<std::is_base_of_v<InputSystem, T>, bool> = true>
concept InputSystemDerived = requires (const sf::Window& window)
{
    /* Only Singletons may be used in InputSystem::GetInstance */
    {T::InitInstance(window)} -> std::convertible_to<InputSystem*>;
};

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

    template <InputSystemDerived Derived>
    static InputSystem* GetInstance(const sf::Window& window) {
        return Derived::InitInstance(window);
    }

    virtual void ReadNewInput() = 0;
    InputTokensArray GetInput() const;

protected:
    explicit InputSystem(const sf::Window& window);
    ~InputSystem();

    InputTokensArray input_tokens_;
    const sf::Window& window_;
private:
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;
};

class KeyboardInputSystem final : public InputSystem {
public:
    void ReadNewInput() override;

    static KeyboardInputSystem* InitInstance(const sf::Window& window);

private:
    explicit KeyboardInputSystem(const sf::Window&);
    KeyboardInputSystem(const KeyboardInputSystem&) = delete;
    KeyboardInputSystem& operator=(const KeyboardInputSystem&) = delete;
};
