#include "GameManagementTools.h"

#include <Core/Engine.h>
#include <Game/GameClasses/Client/Pawns/Maps/Default/MapsDefaultClient.h>
#include <Game/GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.h>
#include <Game/GameClasses/Client/Cerebrates/Characters/CharacterCerebrateClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Mage/CharacterMagePawnClient.h>

#define CRINGE "LET THE FUN BEGIN"

void GameManagementTools::InitGameClient() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<MapsDefaultClient>();
    std::cout << CRINGE << std::endl;
}

void GameManagementTools::InitGameServer() {
    Engine* engine = &Engine::GetInstance();
    engine->CreateGameObjectByDefault<MapsDefaultServer>();
    while (true) {
        std::string input;
        std::cin >> input;
        if (input == CRINGE) {
            break;
        }
    }
    new CharacterCerebrateClient<CharacterDefaultPawnClient>(); // no memory leaks
}