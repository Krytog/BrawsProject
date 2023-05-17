cmake_minimum_required(VERSION 3.16)
project(QtTest)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.8_3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics audio window REQUIRED)

add_executable(QtTest
    main.cpp
    PaintTemplates/BasicSprite.cpp
    PaintTemplates/AnimatedSprite.cpp
    PaintTemplates/VisibleObject.cpp
    PaintTemplates/TextSprite.cpp
    Helpers/CommonHelpers/DrawStaticImageHelper.cpp
    Helpers/CommonHelpers/DrawAnimatedImageHelper.cpp
    Helpers/CommonHelpers/DrawTextHelper.cpp
    RecourcesInitializer/RecourcesInitializer.cpp
    MainWidget/MainWidget.cpp
    MainWidget/Render.cpp
    ../GameObject.cpp
    ../Position.cpp
    ../Vector2D.cpp
    ../MyTime.cpp
    ../EventHandler.cpp
)

target_link_libraries(QtTest PRIVATE Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui
 sfml-graphics sfml-audio sfml-system sfml-window)