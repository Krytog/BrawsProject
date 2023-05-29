cmake_minimum_required(VERSION 3.16)
project(BrawlStars)

set(CMAKE_CXX_STANDARD 20)

# targets and sources
add_executable(Test src/Game/main.cpp)                                # Test target
add_executable(Client_starter src/Game/GameClasses/Client/main.cpp)   # Client Game Starter
add_executable(Client src/Game/GameClasses/Client/Launcher.cpp)       # Game Client
add_executable(Server src/Game/GameClasses/Server/main.cpp)           # Game Server

add_dependencies(Client Client_starter Dup.o)
add_subdirectory("src")

# external libraries
# add_subdirectory(external)

# future testing
# enable_testing()
# add_subdirectory(tests)
