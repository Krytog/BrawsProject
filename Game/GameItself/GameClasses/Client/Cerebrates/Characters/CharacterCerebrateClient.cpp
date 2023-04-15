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

std::string CharacterCerebrateClient::SerializeInfo() const {
    Info actual_info;
    actual_info.current_health = possessed_->GetHealth();
    actual_info.current_pos = possessed_->GetPosition();
    std::string output;
    Serializer::Serialize(actual_info, output);
    return output;
}

void CharacterCerebrateClient::UsePossessedApi(std::string_view serialized_command) const {}