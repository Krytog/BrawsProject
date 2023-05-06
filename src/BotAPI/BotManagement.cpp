#include "BotManagement.h"
#include <iostream>

BotManagement::BotManagement() {
    observer_ = dynamic_cast<Observer*>(Engine::GetInstance().CreateGameObject<Observer>());
}

BotManagement& BotManagement::GetInstance() {
    static BotManagement bot_manager;
    return bot_manager;
}

void BotManagement::ObserveBot(GameObject* bot) {
    observer_->Observe(bot);
}

std::string BotManagement::GetPlaygroundInfo() {
    std::string playground_info;
    auto info_array = observer_->GetScreenCollisions();
    playground_info += std::to_string(info_array.size());  // number of objects on screen
    for (const auto& info: info_array) {
        auto [x, y] = info.position.GetCoordinates();
        playground_info += '{';
        playground_info += info.tag;
        playground_info += '#';
        playground_info += std::to_string(x) + ';' + std::to_string(y);
        playground_info += '}';
    }
    return playground_info;
}

void BotManagement::SendInfoToBot() {
    std::cout << GetPlaygroundInfo() << std::endl;  // output is buffered and flushed
}
