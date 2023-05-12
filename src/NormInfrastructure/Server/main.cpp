#include "Communicator.h"
#include <iostream>

int main() {
    boost::asio::io_context io_context;
    Communicator server(io_context);
    // int one = server.RegUser();
    // int two = server.RegUser();
    // int three = server.RegUser();
    // int counter = 0;

    // while (true) {
    //     server.ReceiveFromClient(one);
    // }
    while (true) {
        int two = server.RegUser();
        io_context.run();
    }
    // io_context.run();

    // while (true) {
    //     auto mess = server.ReceiveFromClient(one);
    //     if (!mess.empty()) {
    //         std::cout << mess << std::endl;
    //     }
    // }

    // while (true) {
    //     auto mess = server.ReceiveFromClient(one);
    //     if (!mess.empty()) {
    //         std::cout << mess << std::endl;
    //     }
    // }
}