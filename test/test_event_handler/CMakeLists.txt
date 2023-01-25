add_gtest(test_event_handler test.cpp)

target_include_directories(event_handler_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(test_event_handler event_handler_lib)