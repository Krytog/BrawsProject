add_subdirectory("Core")
add_subdirectory("Game")
add_subdirectory("SwarmSystem")

add_executable(BrawlStars Game/main.cpp)

target_link_libraries(game core_lib)  # game depends on core
target_link_libraries(BrawlStars PRIVATE core_lib game swarm_system)  # link all to main

# target_include_directories(BrawlStars PRIVATE Core Game SwarmSystem)
target_include_directories(BrawlStars PRIVATE ${CMAKE_CURRENT_LIST_DIR})  # include Core, Game & SwarmSystem

target_link_libraries(BrawlStars PRIVATE sfml-graphics sfml-audio sfml-system sfml-window)