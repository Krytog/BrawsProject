cmake_minimum_required(VERSION 3.16)
project(BrawlStars)

set(CMAKE_CXX_STANDARD 20)

# targets and sources
add_executable(Test src/Game/main.cpp)
add_executable(Client_starter src/Game/GameClasses/Client/main.cpp)
add_executable(Server src/Game/GameClasses/Server/main.cpp)
# target_compile_definitions(Server PRIVATE __SERVER_ENGINE_MODE__)  # use carefully
add_subdirectory("src")

# external libraries
# add_subdirectory(external)

# future testing
# enable_testing()
# add_subdirectory(tests)
