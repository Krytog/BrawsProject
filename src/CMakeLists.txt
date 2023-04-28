add_subdirectory("Core")
add_subdirectory("Game")
add_subdirectory("SwarmSystem")

# game depends on core
target_link_libraries(game_client_tools core_lib)
target_link_libraries(game_server_tools core_lib)
target_link_libraries(game_tools core_lib)
target_link_libraries(game_client core_lib)
target_link_libraries(game_server core_lib)

# link all to main
target_link_libraries(Test PRIVATE core_lib game_tools game_client game_server swarm_system)
target_link_libraries(Client PRIVATE core_lib game_client_tools game_client swarm_system)
target_link_libraries(Server PRIVATE core_lib game_server_tools game_server swarm_system)

# include Core, Game & SwarmSystem
target_include_directories(Test PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Client PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(Server PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# SFML
target_link_libraries(Test PRIVATE sfml-graphics sfml-audio sfml-system sfml-window)
target_link_libraries(Client PRIVATE sfml-graphics sfml-audio sfml-system sfml-window)
target_link_libraries(Server PRIVATE sfml-graphics sfml-audio sfml-system sfml-window)