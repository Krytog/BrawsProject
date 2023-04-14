#include "GameManagementTools.h"

#include "../../Core/Engine.h"
#include "../GameClasses/Maps/Map1/Map1.h"

void GameManagementTools::InitGame() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<Map1>();
    auto ptr = engine->CreateGameObjectByDefault<GameObject>();
    engine->SetCameraOn(ptr);
}