# client tools
add_library(game_starters
        GameClasses/Client/GameStarters/GameStarter.cpp)

add_library(game_client_tools
        Tools/ClientGameManagement.cpp  # special for client
        Tools/Decoy/ServerDecoy.cpp)
# server tools
add_library(game_server_tools
        Tools/ServerGameManagement.cpp  # special for server
        Tools/Decoy/ServerDecoy.cpp)
# test tools
add_library(game_tools
        Tools/ClientGameManagement.cpp  # special for client
        Tools/ServerGameManagement.cpp  # special for server
        )
# game for client & server
add_library(game_client
        GameClasses/Client/Pawns/Maps/Default/MapsDefaultClient.cpp
        GameClasses/Client/Pawns/Maps/LudusMap/LudusMapClient.cpp
        GameClasses/Client/Pawns/Characters/CharacterPawnClient.cpp
        GameClasses/Client/Pawns/Projectiles/ProjectilePawnClient.cpp
        GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.cpp
        GameClasses/Client/Pawns/Characters/Mage/CharacterMagePawnClient.cpp
        GameClasses/Client/Pawns/Characters/Mage/ProjectileMagePawnClient.cpp
        GameClasses/Client/Pawns/Characters/Pirate/CharacterPiratePawnClient.cpp
        GameClasses/Client/Pawns/Characters/Pirate/ProjectilePiratePawnClient.cpp
        GameClasses/Client/Pawns/Characters/Bully/CharacterBullyPawnClient.cpp
        GameClasses/Client/Pawns/Characters/Bully/ProjectileBullyPawnClient.cpp
        GameClasses/Client/Pawns/UI/Cursor.cpp
        GameClasses/Client/Pawns/UI/EndGameWidget.cpp
        GameClasses/Client/Pawns/Particles/Particles.cpp
        GameClasses/Client/Pawns/Particles/ExplosionParticles.cpp
        GameClasses/Client/Pawns/Particles/SmokeParticles.cpp
        GameClasses/Client/Pawns/Particles/HitParticles.cpp
        GameClasses/Client/Pawns/Particles/PowerUpParticles.cpp
        GameClasses/Client/Pawns/Zones/DamageZonePawnClient.cpp
        GameClasses/Client/Pawns/PowerUps/PowerUpPawnClient.cpp
        GameClasses/Client/Pawns/PowerUps/HealthPowerUpPawnClient.cpp
        GameClasses/Client/Pawns/PowerUps/SpeedPowerUpPawnClient.cpp
        GameClasses/Client/Pawns/PowerUps/DamagePowerUpPawnClient.cpp)
add_library(game_server
        GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.cpp
        GameClasses/Server/Pawns/Maps/LudusMap/LudusMap.cpp
        GameClasses/Server/Pawns/Characters/CharacterPawnServer.cpp
        GameClasses/Server/Pawns/Projectiles/ProjectilePawnServer.cpp
        GameClasses/Server/Pawns/Characters/Default/CharacterDefaultPawnServer.cpp
        GameClasses/Server/Pawns/Characters/Mage/CharacterMagePawnServer.cpp
        GameClasses/Server/Pawns/Characters/Mage/ProjectileMagePawnServer.cpp
        GameClasses/Server/Pawns/Characters/Pirate/CharacterPiratePawnServer.cpp
        GameClasses/Server/Pawns/Characters/Pirate/ProjectilePiratePawnServer.cpp
        GameClasses/Server/Pawns/Characters/Bully/CharacterBullyPawnServer.cpp
        GameClasses/Server/Pawns/Characters/Bully/ProjectileBullyPawnServer.cpp
        GameClasses/Server/ParsingUtilities/ControllerTools.cpp
        GameClasses/Server/Pawns/Projectiles/HitTrail.cpp
        GameClasses/Server/GameRulers/GameRuler.cpp
        GameClasses/Server/Pawns/Zones/DamageZonePawnServer.cpp
        GameClasses/Server/Pawns/Zones/HazardZoneManager.cpp
        GameClasses/Server/Pawns/PowerUps/PowerUpPawnServer.cpp
        GameClasses/Server/Pawns/PowerUps/HealthPowerUpPawnServer.cpp
        GameClasses/Server/Pawns/PowerUps/SpeedPowerUpPawnServer.cpp
        GameClasses/Server/Pawns/PowerUps/DamagePowerUpPawnServer.cpp)

target_link_libraries(game_client_tools game_client)
target_link_libraries(game_server_tools game_server)
target_link_libraries(game_tools game_server game_client)

# include Core, Game, SwarmSystem, Infra & BotAPI
target_include_directories(game_client_tools PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(game_server_tools PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(game_tools PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(game_client PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(game_server PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

add_executable(Dup.o
        GameClasses/Client/Dup.c)

