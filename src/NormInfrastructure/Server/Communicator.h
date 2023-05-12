#pragma once

#include <sys/_types/_int64_t.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <random>
#include <boost/asio.hpp>
#include <deque>

namespace {
    const int kMaxDtgrmLen = 1024;
}

using boost::asio::ip::udp;

class Communicator {
    using DataQueue = std::deque<std::string>;
public:
    Communicator(boost::asio::io_context& io_context);
    // static Communicator &GetInstance();

    int64_t RegUser();
    std::string ReceiveFromClient(uint64_t client_id);
    void SendToClient(uint64_t client_id, std::string_view data);

    ~Communicator() {
        io_context_.run();
    } /* сокет тут закрывать не надо */
private:
    // Communicator();

    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    uint64_t RegId();
    void DoRecieve(size_t thread_id);
    bool IsValidData(std::string_view data, int64_t client_id) const;

    boost::asio::io_context io_context_;
    udp::socket socket_;
    udp::socket reg_socket_;
    // udp::socket reg_socket_;

    std::unordered_map<int64_t, udp::endpoint> connections_;
    std::unordered_map<udp::endpoint, int64_t> id_by_connection_;
    std::unordered_map<int64_t, DataQueue> users_data_;
    std::mt19937_64 rand_gen_;

    size_t user_counter_ = 0;
    std::unordered_map<int64_t, udp::endpoint> actual_connections_;
    std::unordered_map<int64_t, std::string> actual_message_;
};