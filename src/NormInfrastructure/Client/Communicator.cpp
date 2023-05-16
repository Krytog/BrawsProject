#include "Communicator.h"
#include <cstddef>
#include <cstring>
#include <iostream>

namespace {
    char host[] = "localhost";
    char random_port[] = "10010";
    char reg_port[] = "10011";
    size_t k_max_dtgrm_len = 3200;
}

Communicator::Communicator(): socket_(io_context_, udp::endpoint(udp::v4(), 0)) {
    udp::resolver resolver(io_context_);
    endpoints_ = resolver.resolve(udp::v4(), host, random_port);
    reg_endpoints_ = resolver.resolve(udp::v4(), host, reg_port);
    package_.resize(k_max_dtgrm_len);
}

Communicator &Communicator::GetInstance() {
    static Communicator instance;
    return instance;
}

void Communicator::DoRecieve() {
    socket_.async_receive_from(boost::asio::buffer(package_, k_max_dtgrm_len), connection_,
                               [this](boost::system::error_code error_code, std::size_t bytes_recvd) { DoRecieve(); });
}

uint64_t Communicator::RegOnServer() {
    char greeting[] = "register";
    socket_.send_to(boost::asio::buffer(greeting, strlen(greeting)), *reg_endpoints_.begin());
    int bytes_recvd = socket_.receive_from(boost::asio::buffer(&user_id_, sizeof(user_id_)), connection_);
    if (bytes_recvd != sizeof(user_id_)) {
        RegOnServer();
    }

    //////////
     std::cout << "Registered with id: " << user_id_ << std::endl;
    /////////

    DoRecieve();

    return user_id_;
}

std::string Communicator::ReceiveFromServer() {
    std::string temp = package_;
    package_.clear();
    package_.resize(k_max_dtgrm_len);
    return temp;
}

void Communicator::SendToServer(std::string_view data) {
    std::string valid_data;
    valid_data.resize(sizeof(user_id_) + data.size());
    memcpy(&valid_data[0], &user_id_, sizeof(user_id_));
    memcpy(&valid_data[0 + sizeof(user_id_)], data.data(), data.size());
    socket_.send_to(boost::asio::buffer(valid_data.data(), valid_data.size()), *endpoints_.begin());
}

void Communicator::RunFor(size_t milliseconds) {
    io_context_.run_for(std::chrono::milliseconds(milliseconds));
}

void Communicator::Run() {
    accept_thread_ = std::thread([this]{ io_context_.run(); });
}

void Communicator::Stop() {
    io_context_.stop();
    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }
}