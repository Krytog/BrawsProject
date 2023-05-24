#include "Communicator.h"
#include <boost/asio/ip/udp.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>

namespace {
    char host[] = "localhost";
    char random_port[] = "10010";
    size_t k_max_dtgrm_len = 3200;
}

Communicator::Communicator(uint16_t port): socket_(io_context_, udp::endpoint(udp::v4(), port)) {
    udp::resolver resolver(io_context_);
    endpoints_ = resolver.resolve(udp::v4(), host, random_port);
    package_.resize(k_max_dtgrm_len);
}

Communicator &Communicator::GetInstance(uint16_t port) {
    static Communicator instance(port);
    return instance;
}

void Communicator::DoReceive() {
    socket_.async_receive_from(boost::asio::buffer(package_, k_max_dtgrm_len), connection_,
                               [this](boost::system::error_code error_code, std::size_t bytes_recvd) { DoReceive(); });
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
    std::cout << user_id_ << " " << valid_data << std::endl;
    // std::cout << (*endpoints_.begin()).endpoint().address() << " " << (*endpoints_.begin()).endpoint().port() << std::endl;
}

void Communicator::Run() {
    DoReceive();
    accept_thread_ = std::thread([this]{ io_context_.run(); });
}

void Communicator::Stop() {
    io_context_.stop();
    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }
}

void Communicator::BindOnPort(uint16_t port) {
    udp::endpoint endpoint(udp::v4(), port);
    std::cout << port << std::endl;
    socket_.bind(endpoint);
    std::cout << "binded" << std::endl;
}

void Communicator::SetId(uint64_t  id) {
    user_id_ = id;
}
