#include "ClientGameManagement.h"

#include <Core/Engine.h>
#include <Game/GameClasses/Client/Pawns/Maps/Default/MapsDefaultClient.h>
#include <Game/GameClasses/Client/Cerebrates/Characters/CharacterCerebrateClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.h>
#include <SwarmSystem/Register.h>
#include <SwarmSystem/TypeIdList.h>

void ClientGameManagement::InitGameClient() {
    Engine& engine = Engine::GetInstance();
    engine.CreateGameObjectByDefault<MapsDefaultClient>();
}

void ClientGameManagement::InitRegistryForOvermind() {
    CerebrateRegistry& registry = CerebrateRegistry::GetInstance();
    registry.RegisterClass<CharacterCerebrateClient<CharacterDefaultPawnClient>>(TypeId_Character_Default);
}

std::string ClientGameManagement::SerializeInput() {
    auto input = Engine::GetInstance().GetInput();
    auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
    input.erase(input.begin());
    Position pos = mouse_token.position;
    std::string output;
    Serializer::Serialize(pos, &output);
    output += std::to_string(mouse_token.key);
    for (auto& token : input) {
        auto keyboard_token = std::get<InputSystem::KeyboardToken>(token);
        output += keyboard_token.symbol;
    }
    return output;
}