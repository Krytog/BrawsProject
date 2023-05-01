add_library(server
        Server/Communicator.cpp BindSocket.c)

add_library(client
        Client/Communicator.cpp BindSocket.c Client/Communicator.h)