#pragma once

#include <string>
#include <Core/Observer.h>
#include <Core/Engine.h>

class BotManagement {
public:
    static BotManagement& GetInstance();

    void ObserveBot(GameObject* bot);

    std::string GetPlaygroundInfo();
    void SendInfoToBot();

private:
    BotManagement();

    Observer* observer_;
};