add_gtest(test_delay_queue test.cpp)

target_include_directories(delay_queue_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_delay_queue delay_queue_lib)