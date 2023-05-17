set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED)

add_library(core_lib
        Collider.cpp
        Colliders.cpp
        DelayQueue.cpp
        EventHandler.cpp
        Position.cpp
        Vector2D.cpp
        CollisionSystem.cpp
        GameObject.cpp
        VisibleGameObject.cpp
        InputSystem.cpp
        MyTime.cpp
        Observer.cpp
        Tools/BasicGeometry.cpp
        Qt/PaintTemplates/AnimatedSprite.cpp
        Qt/PaintTemplates/BasicSprite.cpp
        Qt/PaintTemplates/VisibleObject.cpp
        Qt/PaintTemplates/TextSprite.cpp
        Qt/MainWidget/MainWidget.cpp
        Qt/MainWidget/Render.cpp
        Qt/Helpers/CommonHelpers/DrawAnimatedImageHelper.cpp
        Qt/Helpers/CommonHelpers/DrawStaticImageHelper.cpp
        Qt/Helpers/CommonHelpers/DrawTextHelper.cpp
        Qt/Helpers/CommonHelpers/RotateHelper.cpp)

target_link_libraries(core_lib Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui)