# infra libs for client & server
add_library(infra_client
        Client/Communicator.cpp
        Client/Agent.cpp)

add_library(infra_server
        Server/Communicator.cpp
        Server/Porter.cpp)
        
add_library(infra
        Client/Communicator.cpp
        Client/Agent.cpp
        Server/Communicator.cpp
        Server/Porter.cpp)

target_include_directories(infra_client PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(infra_server PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(infra PRIVATE ${CMAKE_CURRENT_LIST_DIR})
