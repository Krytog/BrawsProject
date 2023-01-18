cmake_minimum_required(VERSION 3.22)
project(server)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS network REQUIRED)

add_executable(server main.cpp)

target_link_libraries(server sfml-network)