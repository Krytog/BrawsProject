#include "InputManagement.h"

#include <Core/Engine.h>
#include <SwarmSystem/Serializer.h>
#include <SwarmSystem/Overmind.h>

std::string InputManagement::SerializeInput() {
    auto input = Engine::GetInstance().GetInput();
    auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
    input.erase(input.begin());
    Position pos = mouse_token.position;
    std::string output;
    Serializer::Serialize(pos, &output);
    for (auto& token : input) {
        auto keyboard_token = std::get<InputSystem::KeyboardToken>(token);
        output += keyboard_token.symbol;
    }
    return output;
}

void InputManagement::HandleInput(uint64_t player_id, std::string_view input) {
    auto cerebrate = Overmind::GetInstance().GetPlayersCerebrate(player_id);
    if (cerebrate) {
        cerebrate->ForcePossessedExecuteCommand(input);
    }
}