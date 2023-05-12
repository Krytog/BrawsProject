add_library(infra_client
        Client/Communicator.cpp)

add_library(infra_server
        Server/Communicator.cpp)
        
add_library(infra
        Client/Communicator.cpp
        Server/Communicator.cpp
        BindSocket.c)