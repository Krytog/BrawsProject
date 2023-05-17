#include "ClientGameManagement.h"

#include <Core/ClientEngine.h>
#include <Game/GameClasses/Client/Pawns/Maps/Default/MapsDefaultClient.h>
#include <Game/GameClasses/Client/Cerebrates/Characters/CharacterCerebrateClient.h>
#include <Game/GameClasses/Client/Cerebrates/Positional/PositionalCerebrateClient.h>
#include <Game/GameClasses/Client/Cerebrates/HitSynchronizer/HitSynchronizerCerebrateClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Mage/CharacterMagePawnClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Mage/ProjectileMagePawnClient.h>
#include <Game/GameClasses/Client/Pawns/UI/Cursor.h>
#include <Game/GameClasses/Client/Pawns/Particles/ExplosionParticles.h>
#include <SwarmSystem/Register.h>
#include <SwarmSystem/TypeIdList.h>

void ClientGameManagement::InitGameClient() {
    ClientEngine& engine = ClientEngine::GetInstance();
    engine.CreateGameObjectByDefault<MapsDefaultClient>();
    engine.CreateGameObjectByDefault<Cursor>();
}

void ClientGameManagement::InitRegistryForOvermind() {
    CerebrateRegistry& registry = CerebrateRegistry::GetInstance();
    registry.RegisterClass<CharacterCerebrateClient<CharacterDefaultPawnClient>>(TypeId_Character_Default);
    registry.RegisterClass<CharacterCerebrateClient<CharacterMagePawnClient>>(TypeId_Character_Mage);
    registry.RegisterClass<PositionalCerebrateClient<ProjectileMagePawnClient>>(TypeId_Projectile_Mage);
    registry.RegisterClass<HitSynchronizerCerebrateClient<ExplosionParticles>>(TypeId_Trail_Explosion);
}

std::string ClientGameManagement::SerializeInput() {
    auto input = ClientEngine::GetInstance().GetInput();
    auto mouse_token = std::get<InputSystem::MouseToken>(*input.begin());
    input.erase(input.begin());
    Position pos = mouse_token.position;
    std::string output;
    Serializer::Serialize(pos, &output);
    output += ('0' + mouse_token.key);
    for (auto& token : input) {
        auto keyboard_token = std::get<InputSystem::KeyboardToken>(token);
        output += keyboard_token.symbol;
    }
    return output;
}