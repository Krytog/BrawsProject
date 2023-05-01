#include "Communicator.h"

Communicator &Communicator::GetInstance() {
    static Communicator instance;
    return instance;
}

extern "C" int BindToSock(char *service);

Communicator::Communicator() {
    sock_fd_ = BindToSock(server_port);
    if (sock_fd_ < 0) {
        throw std::logic_error("Unable to bind port\n");
    }
}

Communicator::~Communicator() {
    if (sock_fd_) {
        close(sock_fd_);
    }
}

void Communicator::RegOnServer() {
    std::string message = "12"; // my_id

    socklen_t servaddr_len = socklen;
    sendto(sock_fd_, message.data(), message.size(), 0, reinterpret_cast<struct sockaddr*>(&server_addr), servaddr_len);
}

std::string Communicator::ReceiveFromServer() {
    std::string cur_message;
    cur_message.resize(kMaxDtgrmLen);
    socklen_t cliaddr_len = socklen;
    std::unique_ptr<struct sockaddr_in> cli_addr(new struct sockaddr_in);
    int n = recvfrom(sock_fd_, &cur_message[0], kMaxDtgrmLen, 0,
                     reinterpret_cast<struct sockaddr *>(cli_addr.get()), &cliaddr_len);
    if (n == -1) {
        return "";
    }
    return cur_message;
}

void Communicator::SendToServer(std::string_view data) {
    sendto(sock_fd_, data.data(), data.size(), 0, reinterpret_cast<const struct sockaddr *>(&server_addr),
           socklen);
}

