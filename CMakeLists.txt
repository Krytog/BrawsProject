cmake_minimum_required(VERSION 3.16)
project(BrawlStars)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio window REQUIRED)

# targets and sources
add_executable(Test src/Game/main.cpp)
add_executable(Client src/Game/GameClasses/Client/main.cpp)
add_executable(Server src/Game/GameClasses/Server/main.cpp)
add_subdirectory("src")

# external libraries
# add_subdirectory(external)

# future testing
# enable_testing()
# add_subdirectory(tests)
