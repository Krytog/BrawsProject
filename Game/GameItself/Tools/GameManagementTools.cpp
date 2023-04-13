#include "GameManagementTools.h"

#include "../../Core/Engine.h"
#include "../GameClasses/Maps/Map1/Map1.h"

void GameManagementTools::InitGame() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<Map1>();
}