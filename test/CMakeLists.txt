add_library(geometry_lib ../Game/Core/Tools/BasicGeometry.cpp)
add_library(vector_lib ../../Game/Core/Vector2D.cpp)
add_library(colliders_lib ../../Game/Core/Colliders.cpp ../../Game/Core/Collider.cpp ../../Game/Core/Position.cpp)

add_subdirectory(libtest)
add_subdirectory(test_Vector2D)
add_subdirectory(test_colliders)