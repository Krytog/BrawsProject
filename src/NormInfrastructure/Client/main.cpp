#include <unistd.h>
#include <string>
#include "Communicator.h"

int main() {

    Communicator::GetInstance().RegOnServer();

    char message[] = "hello";
    int counter = 0;

    while (true) {
        ++counter;
        Communicator::GetInstance().SendToServer(message + std::to_string(counter));
    }
}

