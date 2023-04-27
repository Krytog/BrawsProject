add_library(game
        Tools/ControllerTools.cpp
        Tools/GameManagementTools.cpp
        Tools/InputManagement.cpp
        GameClasses/Client/Pawns/Maps/Default/MapsDefaultClient.cpp
        GameClasses/Client/Pawns/Characters/CharacterPawnClient.cpp
        GameClasses/Client/Pawns/Characters/Default/CharacterDefaultPawnClient.cpp
        GameClasses/Client/Pawns/Characters/Mage/CharacterMagePawnClient.cpp
        GameClasses/Server/Pawns/Characters/Default/CharacterDefaultPawnServer.cpp
        GameClasses/Server/Pawns/Interfaces/IMovable.cpp
        GameClasses/Server/Pawns/Maps/Default/MapsDefaultServer.cpp)
target_include_directories(game PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)  # include Core, Game & SwarmSystem
