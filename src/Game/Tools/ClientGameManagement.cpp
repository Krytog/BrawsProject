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