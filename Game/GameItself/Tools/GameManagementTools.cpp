#include "GameManagementTools.h"

#include "../../Core/Engine.h"
#include "../GameClasses/Common/Maps/Map1/Map1.h"
#include "../GameClasses/Client/Pawns/Characters/DefaultCharacter/DefaultCharacterPawnClient.h"
#include "../GameClasses/Client/Cerebrates/Characters/CharacterCerebrateClient.h"
#include "../../SwarmSystem/Serializer.h"

void GameManagementTools::InitGame() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<Map1>();
    auto player = engine->CreateGameObject<DefaultCharacterPawnClient>(Position(-500, -50));
    engine->SetCameraOn(player);
}