add_gtest(test_colliders test.cpp)

target_include_directories(colliders_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_colliders colliders_lib vector_lib geometry_lib)