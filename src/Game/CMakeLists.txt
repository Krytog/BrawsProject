add_library(game
        Tools/ControllerTools.cpp
        Tools/GameManagementTools.cpp
        GameClasses/Common/Maps/Map1/Map1.cpp
        GameClasses/Client/Pawns/Characters/CharacterPawnClient.cpp
        GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.cpp
        GameClasses/Client/Pawns/Characters/Mage/CharacterMagePawnClient.cpp
        GameClasses/Server/Pawns/Characters/Default/CharacterDefaultPawnServer.cpp
        GameClasses/Server/Pawns/Interfaces/IMovable.cpp)
target_include_directories(game PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)  # include Core, Game & SwarmSystem
