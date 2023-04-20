#include "CharacterCerebrateClient.h"
#include "../../Pawns/Characters/DefaultCharacter/DefaultCharacterPawnClient.h"

#include "../../../../../SwarmSystem/Serializer.h"
#include "../../../../../SwarmSystem/CerebratesTypeId.h"

CharacterCerebrateClient::CharacterCerebrateClient(CharacterPawnClient *pawn_to_possess, bool is_controllable):
    possessed_(pawn_to_possess),
    is_controllable_(is_controllable), Cerebrate(TypeId_CharacterCerebrateClient) {
}

CharacterCerebrateClient::~CharacterCerebrateClient() = default;

void CharacterCerebrateClient::ForcePossessedExecuteCommand(const std::string &serialized_command) const {
    Info actual_info;
    auto info_size = sizeof(actual_info);
    Serializer::Deserialize(actual_info, serialized_command.substr(0, info_size));
    UsePossessedApi(serialized_command.substr(info_size));
    possessed_->UpdatePosition(actual_info.current_pos);
    possessed_->SetHealth(actual_info.current_health);
}

std::string CharacterCerebrateClient::SerializeInfo() {
    if (is_controllable_) {
        HandleInput();
    }
    Info actual_info;
    actual_info.current_health = possessed_->GetHealth();
    actual_info.current_pos = possessed_->GetPosition();
    std::string output;
    Serializer::Serialize(actual_info, output);
    return output;
}

void CharacterCerebrateClient::UsePossessedApi(std::string_view serialized_command) const {}

void CharacterCerebrateClient::HandleInput() {
    auto input = Engine::GetInstance().GetInput();
    std::string commands;
    commands.reserve(4);
    for (auto& token : input) {
        auto keyboard_token = std::get_if<InputSystem::KeyboardToken>(&token);
        if (!keyboard_token) {
            continue;
        }
        if (keyboard_token->symbol == 'W') {
            commands += 'W';
        } else if (keyboard_token->symbol == 'A') {
            commands += 'A';
        } else if (keyboard_token->symbol == 'S') {
            commands += 'S';
        } else if (keyboard_token->symbol == 'D') {
            commands += 'D';
        }
    }
    AddCommandToBuffer(commands);
}