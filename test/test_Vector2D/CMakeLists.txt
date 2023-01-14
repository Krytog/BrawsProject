add_gtest(test_vector2d test.cpp)

add_library(vector_lib ../../Game/Core/Vector2D.cpp)

target_include_directories(vector_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_vector2d vector_lib)