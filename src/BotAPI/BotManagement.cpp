#include "BotManagement.h"
#include <iostream>

#include <Core/ClientEngine.h>

BotManagement::BotManagement() {
    observer_ = dynamic_cast<Observer*>(ClientEngine::GetInstance().CreateGameObject<Observer>());
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
    auto collisions_with_player = observer_->GetScreenCollisions();
    auto info_array = collisions_with_player.info_array;
    auto player_id = collisions_with_player.player_index;
    playground_info += std::to_string(info_array.size());  // number of objects on screen
    playground_info += '#';
    playground_info += std::to_string(player_id);  // player ID in the info array, starting from 0; -1 <=> NO
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
