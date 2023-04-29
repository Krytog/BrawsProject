# client tools
add_library(game_client_tools
        Tools/ClientGameManagement.cpp  # special for client
        Tools/ControllerTools.cpp
        Tools/InputManagement.cpp
        Tools/Decoy/ServerDecoy.cpp)
# server tools
add_library(game_server_tools
        Tools/ServerGameManagement.cpp  # special for server
        Tools/ControllerTools.cpp
        Tools/InputManagement.cpp
        Tools/Decoy/ServerDecoy.cpp)
# test tools
add_library(game_tools
        Tools/ClientGameManagement.cpp  # special for client
        Tools/ServerGameManagement.cpp  # special for server
        Tools/ControllerTools.cpp
        Tools/InputManagement.cpp)
# game for client & server
add_library(game_client
        GameClasses/Client/Pawns/Maps/Default/MapsDefaultClient.cpp
        GameClasses/Client/Pawns/Characters/CharacterPawnClient.cpp
        GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.cpp
        GameClasses/Client/Pawns/Characters/Mage/CharacterMagePawnClient.cpp)
add_library(game_server
        GameClasses/Server/Pawns/Characters/CharacterPawnServer.cpp
        GameClasses/Server/Pawns/Characters/Default/CharacterDefaultPawnServer.cpp
        GameClasses/Server/Pawns/Interfaces/IMovable.cpp
        GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.cpp)

target_link_libraries(game_client_tools game_client)
target_link_libraries(game_server_tools game_server)
target_link_libraries(game_tools game_server game_client)

# include Core, Game & SwarmSystem
target_include_directories(game_client_tools PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(game_server_tools PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(game_tools PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(game_client PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(game_server PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)

