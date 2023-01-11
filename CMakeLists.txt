cmake_minimum_required(VERSION 3.16)
project(BrawlStars)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio window REQUIRED)

add_executable(BrawlStars Game/Core/main.cpp Game/Core/MyTime.h Game/Core/MyTime.cpp
        Game/Core/Position.h Game/Core/Vector2D.h Game/Core/Position.cpp
        Game/Core/Vector2D.cpp Game/Core/Collider.cpp Game/Core/Collider.h
        Game/Core/Colliders.cpp Game/Core/Colliders.h Game/Core/Canvas.h
        Game/Core/Canvas.cpp Game/Core/Sprites.h Game/Core/Sprites.cpp
        Game/Core/VisibleObject.h Game/Core/GameObject.h Game/Core/GameObject.cpp Game/Core/Render.cpp
        Game/Core/Tools/BasicGeometry.cpp Game/Core/CollisionSystem.cpp)

target_link_libraries(BrawlStars sfml-graphics sfml-audio sfml-system sfml-window)