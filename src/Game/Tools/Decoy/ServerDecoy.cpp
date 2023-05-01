#include "ServerDecoy.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#define HELLA_A_LOT 10000

std::string DecoyServer::Receive() {
    size_t size;
    std::cin >> size;
    char buffer[HELLA_A_LOT];
    int fd = open("kek.txt", O_RDONLY);
    read(fd, buffer, size);
    close(fd);
    std::string output;
    output.resize(size);
    for (size_t i = 0; i < size; ++i) {
        output[i] = buffer[i];
    }
    return output;
}

void DecoyServer::Send(std::string_view data) {
    std::cout << data.size() << std::endl;
    int fd = open("kek.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    write(fd, data.data(), data.size());
    close(fd);
}