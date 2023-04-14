#include "GameManagementTools.h"

#include "../../Core/Engine.h"
#include "../GameClasses/Common/Maps/Map1/Map1.h"
#include "../GameClasses/Client/Pawns/Characters/DefaultCharacter/DefaultCharacterPawnClient.h"

void GameManagementTools::InitGame() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<Map1>();
    auto player = engine->CreateGameObject<>();
    engine->SetCameraOn(player);
}