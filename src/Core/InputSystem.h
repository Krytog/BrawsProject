#pragma once

#include "Position.h"

#include <variant>
#include <cstdint>
#include <list>
#include <memory>

class InputSystem;

template<typename T, std::enable_if_t<std::is_base_of_v<InputSystem, T>, bool> = true>
concept InputSystemDerived = requires ()
{
    /* Only Singletons may be used in InputSystem::GetInstance */
    {T::InitInstance()} -> std::convertible_to<InputSystem*>;
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
    static InputSystem* GetInstance() {
        return Derived::InitInstance();
    }

    virtual void ReadNewInput() = 0;
    InputTokensArray GetInput() const;

protected:
    InputSystem() = default;
    ~InputSystem();

    InputTokensArray input_tokens_;
private:
    InputSystem(const InputSystem&) = delete;
    InputSystem& operator=(const InputSystem&) = delete;
};

class KeyboardInputSystem final : public InputSystem {
public:
    void ReadNewInput() override;

    static KeyboardInputSystem* InitInstance();

private:
    KeyboardInputSystem() = default;
    KeyboardInputSystem(const KeyboardInputSystem&) = delete;
    KeyboardInputSystem& operator=(const KeyboardInputSystem&) = delete;
};
