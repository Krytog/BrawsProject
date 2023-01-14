option(TEST_SOLUTION "Build solution" OFF)

function(add_custom_executable NAME)
    add_executable(${NAME} ${ARGN})
    target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

function(add_custom_test_binary TARGET)
    add_custom_executable(${TARGET} ${ARGN})

    if (TEST_SOLUTION)
        add_custom_target(
                run_${TARGET}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${TARGET}
                COMMAND ${CMAKE_BINARY_DIR}/${TARGET})

        add_dependencies(test-all run_${TARGET})
    endif ()
endfunction()

function(add_gtest TARGET)
    add_custom_test_binary(${TARGET}
            ${CMAKE_SOURCE_DIR}/contrib/gmock_main.cc
            ${ARGN})

    target_link_libraries(${TARGET} gmock)
endfunction()

add_custom_target(test-all)


