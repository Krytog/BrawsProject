#pragma once

#include "../Core/InputSystem.h"

class BotInputSystem : public InputSystem {
public:
    void ReadNewInput() override;

    static BotInputSystem* InitInstance();
private:
    BotInputSystem() = default;
    BotInputSystem(const BotInputSystem&) = delete;
    BotInputSystem& operator=(const BotInputSystem&) = delete;
};