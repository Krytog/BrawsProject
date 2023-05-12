#include <unistd.h>
#include <string>
#include "Communicator.h"

int main() {
    boost::asio::io_context io_context;

    Communicator client(io_context);
    client.RegOnServer();


    // char message[] = "hello";
    // int counter = 0;

    // while (true) {
    //     ++counter;
    //     client.SendToServer(message + std::to_string(counter));
    // }

    io_context.run();
}

