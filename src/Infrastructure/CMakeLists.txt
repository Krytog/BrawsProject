# infra libs for client & server
add_library(infra_client
        Client/Communicator.cpp
        BindSocket.c)

add_library(infra_server
        Server/Communicator.cpp
        BindSocket.c)
add_library(infra
        Client/Communicator.cpp
        Server/Communicator.cpp
        BindSocket.c)
