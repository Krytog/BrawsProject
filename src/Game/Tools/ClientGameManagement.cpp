#include "ClientGameManagement.h"

#include <Core/ClientEngine.h>
#include <Game/GameClasses/Client/Pawns/Maps/Default/MapsDefaultClient.h>
#include <Game/GameClasses/Client/Cerebrates/Characters/CharacterCerebrateClient.h>
#include <Game/GameClasses/Client/Cerebrates/Positional/PositionalCerebrateClient.h>
#include <Game/GameClasses/Client/Cerebrates/HitSynchronizer/HitSynchronizerCerebrateClient.h>
#include <Game/GameClasses/Client/Cerebrates/Zone/ZoneCerebrateClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Mage/CharacterMagePawnClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Mage/ProjectileMagePawnClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Pirate/CharacterPiratePawnClient.h>
#include <Game/GameClasses/Client/Pawns/Characters/Pirate/ProjectilePiratePawnClient.h>
#include <Game/GameClasses/Client/Pawns/UI/Cursor.h>
#include <Game/GameClasses/Client/Pawns/UI/EndGameWidget.h>
#include <Game/GameClasses/Client/Pawns/Particles/ExplosionParticles.h>
#include <Game/GameClasses/Client/Pawns/Particles/SmokeParticles.h>
#include <Game/GameClasses/Client/Pawns/Zones/DamageZonePawnClient.h>
#include <SwarmSystem/Register.h>
#include <SwarmSystem/TypeIdList.h>
#include <NormInfrastructure/Client/Communicator.h>

#define WIN_STRING "!WIN!"
#define LOSE_STRING "!LOSE!"
#define WIN_LENGTH 5
#define LOSE_LENGTH 6

namespace {
    std::string_view DataPreprocessing(std::string_view data) {
        static bool widget_created = false;
        if (data.starts_with(WIN_STRING)) {
            if (!widget_created) {
                ClientEngine::GetInstance().CreateGameObject<EndGameWidget>(EndGameWidget::Status::WIN);
                widget_created = true;
            }
            return data.substr(WIN_LENGTH);
        }
        if (data.starts_with(LOSE_STRING)) {
            if (!widget_created) {
                ClientEngine::GetInstance().CreateGameObject<EndGameWidget>(EndGameWidget::Status::LOSE);
                widget_created = true;
            }
            return data.substr(LOSE_LENGTH);
        }
        return data;
    }
}

void ClientGameManagement::InitGameClient() {
    ClientEngine& engine = ClientEngine::GetInstance();
    engine.CreateGameObjectByDefault<MapsDefaultClient>();
    engine.CreateGameObjectByDefault<Cursor>();
}

void ClientGameManagement::InitRegistryForOvermind() {
    CerebrateRegistry& registry = CerebrateRegistry::GetInstance();
    registry.RegisterClass<CharacterCerebrateClient<CharacterDefaultPawnClient>>(TypeId_Character_Default);
    registry.RegisterClass<CharacterCerebrateClient<CharacterMagePawnClient>>(TypeId_Character_Mage);
    registry.RegisterClass<CharacterCerebrateClient<CharacterPiratePawnClient>>(TypeId_Character_Pirate);
    registry.RegisterClass<PositionalCerebrateClient<ProjectileMagePawnClient>>(TypeId_Projectile_Mage);
    registry.RegisterClass<PositionalCerebrateClient<ProjectilePiratePawnClient>>(TypeId_Projectile_Pirate);
    registry.RegisterClass<PositionalCerebrateClient<ProjectilePiratePawnClient::Mark>>(TypeId_Projectile_Mark);
    registry.RegisterClass<HitSynchronizerCerebrateClient<ExplosionParticles>>(TypeId_Trail_Explosion);
    registry.RegisterClass<HitSynchronizerCerebrateClient<SmokeParticles>>(TypeId_Trail_Smoke);
    registry.RegisterClass<ZoneCerebrateClient<DamageZonePawnClient>>(TypeId_Zone_Damage);
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

void ClientGameManagement::ReceiveAndHandleFromServer() {
    static Overmind& overmind = Overmind::GetInstance();
    static Communicator& communicator = Communicator::GetInstance();
    auto raw_data = communicator.ReceiveFromServer();
    std::string_view data(raw_data);
    data = DataPreprocessing(data);
    if (data[0] == '#') { // means that we probably have a good package
        overmind.ActualizeCerebrates(data);
        overmind.ForceCerebratesExecuteCommands(data);
    }
}