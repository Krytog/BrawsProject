#include "Communicator.h"
#include <iostream>


void Func() {
    while (true) {
        std::cout << "some" << std::endl;
    }
}

int main() {
    // int one = server.RegUser();
    // int two = server.RegUser();
    // int three = server.RegUser();
    // int counter = 0;

    // while (true) {
    //     server.ReceiveFromClient(one);
    // }
    int64_t one = Communicator::GetInstance().RegUser();
    int64_t two = Communicator::GetInstance().RegUser();

    while (true) {
        auto message = Communicator::GetInstance().ReceiveFromClient(one);
        if (!message.empty()) {
            std::cout << message << " " << one << std::endl;
        }

        auto message2 = Communicator::GetInstance().ReceiveFromClient(two);
        if (!message2.empty()) {
            std::cout << message2 << " " << two << std::endl;
        }
        
        Communicator::GetInstance().RunFor(5);
    }
    // while (true) {
    //     int two = server.RegUser();

    //     std::cout << "safasdf";
    // }
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