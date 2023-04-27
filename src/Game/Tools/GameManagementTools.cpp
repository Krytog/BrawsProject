#include "GameManagementTools.h"

#include "../../Core/Engine.h"
#include "../GameClasses/Common/Maps/Map1/Map1.h"
#include "../GameClasses/Client/Cerebrates/Characters/CharacterCerebrateClient.h"
#include "../GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.h"
#include "../GameClasses/Client/Pawns/Characters/Mage/CharacterMagePawnClient.h"

void GameManagementTools::InitGameClient() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<Map1>();
    auto view_port = engine->CreateGameObjectByDefault<PersistentObject>();
    new CharacterCerebrateClient<CharacterDefaultPawnClient>(); // no memory leaks
    new CharacterCerebrateClient<CharacterMagePawnClient>(); // no memory leaks
    engine->SetCameraOn(view_port);
}

void GameManagementTools::InitGameServer() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<Map1>();
    auto view_port = engine->CreateGameObjectByDefault<PersistentObject>();
    new CharacterCerebrateClient<CharacterDefaultPawnClient>(); // no memory leaks
    engine->SetCameraOn(view_port);
}