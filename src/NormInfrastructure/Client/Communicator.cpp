#include "Communicator.h"
#include <cstddef>
#include <cstring>
#include <iostream>

namespace {
    char host[] = "localhost";
    char random_port[] = "10010";
    char reg_port[] = "10011";
    size_t k_max_dtgrm_len = 1024;
}

Communicator::Communicator(boost::asio::io_context& io_context): socket_(io_context, udp::endpoint(udp::v4(), 0)) {
    udp::resolver resolver(io_context_);
    endpoints_ = resolver.resolve(udp::v4(), host, random_port);
    reg_endpoints_ = resolver.resolve(udp::v4(), host, reg_port);
}

// Communicator &Communicator::GetInstance() {
//     static Communicator instance;
//     return instance;
// }

// Communicator::Communicator(): socket_(io_context_, udp::endpoint(udp::v4(), 0)) {
//     udp::resolver resolver(io_context_);
//     endpoints_ = resolver.resolve(udp::v4(), host, random_port);
// }

void Communicator::DoRecieve() {
    packages_.emplace_back();
    packages_.back().resize(k_max_dtgrm_len);

    socket_.async_receive_from(
        boost::asio::buffer(packages_.back(), k_max_dtgrm_len), connection_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                std::cout << "Recieved " << packages_.back() << std::endl; /* Debug */
                DoRecieve();
    });
}

int64_t Communicator::RegOnServer() {
    char greeting[] = "register";
    socket_.send_to(boost::asio::buffer(greeting, strlen(greeting)), *reg_endpoints_.begin());
    int bytes_recvd = socket_.receive_from(boost::asio::buffer(&user_id_, sizeof(user_id_)), connection_);
    std::cout << bytes_recvd << std::endl;
    if (bytes_recvd != sizeof(user_id_)) {
        RegOnServer();
    }

    std::cout << "Registered with id: " << user_id_ << std::endl;
    return user_id_;
}

std::string Communicator::ReceiveFromServer() {
    if (packages_.empty()) {
        return "";
    }
    std::string message = std::move(packages_.front());
    packages_.pop_front();
    return std::move(message);
}

void Communicator::SendToServer(std::string_view data) {
    std::string valid_data;
    valid_data.resize(sizeof(user_id_) + data.size());
    memcpy(&valid_data[0], &user_id_, sizeof(user_id_));
    memcpy(&valid_data[0 + sizeof(user_id_)], data.data(), data.size());
    int bytes = socket_.send_to(boost::asio::buffer(data.data(), data.size()), *endpoints_.begin());
}