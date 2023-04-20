#include "CharacterCerebrateServer.h"

#include "../../Pawns/Characters/CharacterPawnServer.h"

#include "../../../../../SwarmSystem/Serializer.h"
#include "../../../../../SwarmSystem/CerebratesTypeId.h"

CharacterCerebrateServer::CharacterCerebrateServer(CharacterPawnServer *pawn_to_possess):
        possessed_(pawn_to_possess), Cerebrate(TypeId_CharacterCerebrateClient) {}

CharacterCerebrateServer::~CharacterCerebrateServer() = default;

void CharacterCerebrateServer::ForcePossessedExecuteCommand(const std::string &serialized_command) const {
    Vector2D input;
    for (auto s : serialized_command) {
        if (s == 'W') {
            input += Vector2D::Up;
        } else if (s == 'A') {
            input += Vector2D::Left;
        } else if (s == 'S') {
            input += Vector2D::Down;
        } else if (s == 'D') {
            input += Vector2D::Right;
        }
    }
    input.Normalize();
    possessed_->Move(input);
}

std::string CharacterCerebrateServer::SerializeInfo() {
    Info actual_info;
    actual_info.current_health = possessed_->GetHealth();
    actual_info.current_pos = possessed_->GetPosition();
    std::string output;
    Serializer::Serialize(actual_info, output);
    return output;
}

void CharacterCerebrateServer::UsePossessedApi(std::string_view serialized_command) const {}