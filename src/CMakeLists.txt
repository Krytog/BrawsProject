set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)

add_subdirectory("Core")
add_subdirectory("Game")
add_subdirectory("SwarmSystem")
add_subdirectory("NormInfrastructure")
add_subdirectory("BotAPI")
add_subdirectory("Gui2")

# game depends on core
target_link_libraries(game_client_tools core_lib)
target_link_libraries(game_server_tools core_lib)
target_link_libraries(game_tools core_lib)
target_link_libraries(game_client core_lib)
target_link_libraries(game_server core_lib)

# bot API linking
target_link_libraries(bot_api core_lib)
target_link_libraries(game_client bot_api)

# link all to main
target_link_libraries(Test PRIVATE core_lib game_tools game_client game_server swarm_system infra bot_api)
target_link_libraries(Client_starter PRIVATE core_lib game_client_tools game_client swarm_system infra_client bot_api)
target_link_libraries(Server PRIVATE core_lib game_server_tools game_server swarm_system infra_server)

# include Core, Game, SwarmSystem, Infra & BotAPI
target_include_directories(Test PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Client_starter PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Server PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Qt
target_link_libraries(Test PRIVATE Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui)
target_link_libraries(Client_starter PRIVATE Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui)
