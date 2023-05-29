#include "Communicator.h"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>
#include "GameInfo.h"

Communicator::Communicator()
    : socket_(io_context_, udp::endpoint(udp::v4(), COMMUNICATOR_RECEIVE_PORT)),
      rd_(),
      gen_(rd_()),
      dis_() {
}

Communicator::~Communicator() {
    if (!io_context_.stopped()) {
        Stop();
    }
}

Communicator& Communicator::GetInstance() {
    static Communicator instance;
    return instance;
}

void Communicator::SendToClient(uint64_t client_id, std::string_view data) {
    if (endpoint_by_id_.contains(client_id)) {
        socket_.send_to(boost::asio::buffer(data.data(), data.size()), endpoint_by_id_[client_id]);
    }
}

void Communicator::DoReceive(uint64_t thread_id) {
    actual_message_[thread_id].resize(kMaxDtgrmLen);

    socket_.async_receive_from(
        boost::asio::buffer(actual_message_[thread_id], kMaxDtgrmLen), actual_connections_[thread_id],
        [this, thread_id](boost::system::error_code error_code, std::size_t bytes_recvd) {
            // TODO: тут тоже добавить проверку с мапой, просто в тестинге ip одинаковый
            uint64_t usr_id;
            std::memcpy(&usr_id, actual_message_[thread_id].data(), sizeof(usr_id));
            if (!players_.contains(usr_id)) { // Not lobby player
                DoReceive(usr_id);
                return;
            }
            users_data_[usr_id].push(std::move(actual_message_[thread_id]));
            endpoint_by_id_[usr_id] = actual_connections_[thread_id]; // Update connection
            DoReceive(thread_id);
        });
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

void Communicator::Run() {
    for (auto user_id: players_) {
        DoReceive(user_id);
    }
    accept_thread_ = std::thread([this] { io_context_.run(); });
}

void Communicator::Stop() {
    io_context_.stop();
    if (accept_thread_.joinable()) {
        accept_thread_.join();
    }
}

std::vector<uint64_t> Communicator::SetClients(const std::unordered_map<uint64_t, Player>& players) {
    std::vector<uint64_t> res;
    for (const auto& [id, player] : players) {
        res.push_back(id);
        players_.insert(id);
    }
    return res;
}