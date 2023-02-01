add_gtest(test_basic_sequencer test.cpp)

target_include_directories(sprites_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_basic_sequencer sprites_lib colliders_lib vector_lib geometry_lib)