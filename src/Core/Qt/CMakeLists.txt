cmake_minimum_required(VERSION 3.16)
project(QtTest)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics audio window REQUIRED)

add_executable(QtTest
    main.cpp
    PaintTemplates/BasicSprite.cpp
    PaintTemplates/VisibleObject.cpp
    MainWidget/MainWidget.cpp
    Helpers/CommonHelpers/DrawStaticImageHelper.cpp
    ../GameObject.cpp
    ../Position.cpp
    ../Vector2D.cpp
)

target_link_libraries(QtTest PRIVATE Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui GL
 sfml-graphics sfml-audio sfml-system sfml-window)