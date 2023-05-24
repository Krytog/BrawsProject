#include "Communicator.h"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>

namespace  {
    size_t random_port = 10010;
    size_t reg_port = 10011;
}

Communicator::Communicator(): socket_(io_context_, udp::endpoint(udp::v4(), random_port)),
      reg_socket_(io_context_, udp::endpoint(udp::v4(), reg_port)), rd_(), gen_(rd_()), dis_() {
}

Communicator::~Communicator() {
    if (!io_context_.stopped()) {
        Stop();
    }
}

Communicator &Communicator::GetInstance() {
    static Communicator instance;
    return instance;
}

uint64_t Communicator::RegId() {
    do {
        uint64_t num = dis_(gen_);
        if (!connections_.contains(num)) {
            return num;
        }
    } while (true);
}

uint64_t Communicator::RegUser() {
    uint64_t usr_id = RegId();

    char message[kMaxDtgrmLen];
    size_t bytes_recvd = reg_socket_.receive_from(
        boost::asio::buffer(message, kMaxDtgrmLen), connections_[usr_id]);
    std::string_view reg_message = "register";
    if ((bytes_recvd != reg_message.size()) /* || (reg_message != actual_message_[usr_id].substr(0, reg_message.size())) */) {
        char bad_reg[] = "";
        reg_socket_.send_to(boost::asio::buffer(bad_reg, strlen(bad_reg)), connections_[usr_id]);
        return RegUser();
    }
    //////////
    std::cout << "Registered user with id: " << usr_id << std::endl;
    /////////

    char payload[sizeof(usr_id)];
    memcpy(payload, &usr_id, sizeof(usr_id));
    reg_socket_.send_to(boost::asio::buffer(payload, strlen(payload)), connections_[usr_id]);

    DoReceive(usr_id);

    ++user_counter_;
    return usr_id;
}

void Communicator::SendToClient(uint64_t client_id, std::string_view data) {
    socket_.send_to(boost::asio::buffer(data.data(), data.size()), players_[client_id].endpoint);
}

void Communicator::DoReceive(uint64_t thread_id) {
    actual_message_[thread_id].resize(kMaxDtgrmLen); /* убрать потом  */

    socket_.async_receive_from(
        boost::asio::buffer(actual_message_[thread_id], kMaxDtgrmLen), actual_connections_[thread_id],
        [this, thread_id](boost::system::error_code error_code, std::size_t bytes_recvd) {
            uint64_t usr_id;
            std::memcpy(&usr_id, actual_message_[thread_id].data(), sizeof(usr_id));
            users_data_[usr_id].push(std::move(actual_message_[thread_id]));
          DoReceive(thread_id);
        }
    );
}

bool Communicator::IsValidData(std::string_view data, uint64_t client_id) const {
    uint64_t id;
    std::memcpy(&id, data.data(), sizeof(id));
    if (id == client_id) {
        return true;
    }
    return false;
}

std::string Communicator::ReceiveFromClient(uint64_t client_id) {
    if (users_data_[client_id].empty() || !IsValidData(users_data_[client_id].front(), client_id)) {
        if (!users_data_[client_id].empty()) {
            users_data_[client_id].pop();
        }
        return "";
    }

    std::string message = std::move(users_data_[client_id].front());
    users_data_[client_id].pop();
    return std::move(message.substr(sizeof(client_id)));
}

void Communicator::RunFor(size_t milliseconds) {
    io_context_.run_for(std::chrono::milliseconds(milliseconds));
}

void Communicator::Run() {
    DoReceive(1);
    accept_thread_ = std::thread([this]{ io_context_.run(); });
}

void Communicator::Stop() {
    io_context_.stop();
    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }
}

size_t Communicator::GetUserNumber() {
    return user_counter_;
}

std::vector<uint64_t> Communicator::SetClients(const std::unordered_map<uint64_t, Player>& players) {
    players_ = players;
    std::vector<uint64_t> res;
    for (const auto& [id, player]: players_) {
        res.push_back(id);
    }
    return res;
}
