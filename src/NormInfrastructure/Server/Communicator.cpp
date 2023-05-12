#include "Communicator.h"
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

    actual_message_[usr_id].resize(kMaxDtgrmLen);
    reg_socket_.async_receive_from(
        boost::asio::buffer(actual_message_[usr_id], kMaxDtgrmLen), connections_[usr_id],
        [this, usr_id](boost::system::error_code error_code, std::size_t bytes_recvd) {
            std::string reg_message = "register";
            if (error_code || (bytes_recvd != reg_message.size()) || 
                    (reg_message != actual_message_[usr_id].substr(0, reg_message.size()))) {
                char bad_reg[] = "";
                reg_socket_.send_to(boost::asio::buffer(bad_reg, strlen(bad_reg)), connections_[usr_id]); 
                actual_message_[usr_id].clear();
                return;
            }
            ////////////
            // std::cout << "Registered user with id: " << usr_id << std::endl;
            ///////////
            
            users_data_[usr_id].resize(10);
            // id_by_connection_[connections_[usr_id]] = usr_id;
            char payload[sizeof(usr_id)];
            memcpy(payload, &usr_id, sizeof(usr_id));
            reg_socket_.send_to(boost::asio::buffer(payload, strlen(payload)), connections_[usr_id]);

            DoRecieve(usr_id);
        }
    );

    return usr_id;
}

void Communicator::SendToClient(uint64_t client_id, std::string_view data) {
    socket_.async_send_to(boost::asio::buffer(data.data(), data.size()), connections_[client_id],
                    [this](boost::system::error_code, std::size_t) { /* do nothing yet */ });
}

void Communicator::DoRecieve(size_t thread_id) {
    actual_message_[thread_id].resize(kMaxDtgrmLen); /* убрать потом  */

    socket_.async_receive_from(
        boost::asio::buffer(actual_message_[thread_id], kMaxDtgrmLen), actual_connections_[thread_id],
        [this, thread_id](boost::system::error_code error_code, std::size_t bytes_recvd) {
            // if (!id_by_connection_.contains(actual_connections_[thread_id])) { /* Кто-то левый шлёт */
            //     socket_.send_to(boost::asio::buffer("", 0), actual_connections_[thread_id]);
            //     return;
            // }
            
            uint64_t usr_id;
            std::memcpy(&usr_id, actual_message_[thread_id].data(), sizeof(usr_id));
            users_data_[usr_id].push_back(std::move(actual_message_[thread_id]));
        
            //////////////////////
            // std::cout << user_id << " " << bytes_recvd << " " <<  users_data_[user_id].back() << std::endl;
            //////////////////////

            DoRecieve(thread_id);
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
    if (users_data_[client_id].empty() || !IsValidData(users_data_[client_id].front().data(), client_id)) {
        if (!users_data_[client_id].empty()) {
            users_data_[client_id].pop_front();
        }
        return "";
    }

    std::string message = std::move(users_data_[client_id].front());
    users_data_[client_id].pop_front();
    return std::move(message.substr(sizeof(client_id)));
}

void Communicator::RunFor(size_t milliseconds) {
     io_context_.run_for(std::chrono::milliseconds(milliseconds));
}
