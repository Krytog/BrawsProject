#include <cstring>

#include "Communicator.h"

Communicator &Communicator::GetInstance() {
    static Communicator instance;
    return instance;
}

extern "C" int BindToSock(char *service);

Communicator::Communicator() {
    sock_fd_ = BindToSock(my_port);
    if (sock_fd_ < 0) {
        throw std::logic_error("Unable to bind port\n");
    }
}

Communicator::~Communicator() {
    if (sock_fd_) {
        close(sock_fd_);
    }
}

int64_t Communicator::RegOnServer() {
    sendto(sock_fd_, nullptr, 0, MSG_CONFIRM, reinterpret_cast<struct sockaddr*>(&server_addr), socklen);

    recv(sock_fd_, &usr_id_, sizeof(usr_id_), 0);
    return usr_id_;
}

std::string Communicator::ReceiveFromServer() {
    std::string cur_message;
    cur_message.resize(kMaxDtgrmLen);
    int n = recv(sock_fd_, &cur_message[0], kMaxDtgrmLen, MSG_DONTWAIT);
    if (n == -1 || n == 0) {
        return "";
    }
    cur_message.resize(n);
    return std::move(cur_message);
}

void Communicator::SendToServer(std::string_view data) {
    std::string my_data;
    my_data.resize(sizeof(usr_id_) + data.size());
    memcpy(&my_data[0], &usr_id_, sizeof(usr_id_));
    memcpy(&my_data[0 + sizeof(usr_id_)], data.data(), data.size());
    sendto(sock_fd_, my_data.data(), my_data.size(), 0, reinterpret_cast<const struct sockaddr *>(&server_addr),
           socklen);
}

