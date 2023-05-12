#pragma once

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
    static Communicator &GetInstance();

    int64_t RegUser();
    std::string ReceiveFromClient(int64_t client_id);
    void SendToClient(int64_t client_id, std::string_view data);
    void RunFor(size_t milliseconds);

    ~Communicator() = default;
private:
    Communicator();

    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    uint64_t RegId();
    void DoRecieve(size_t thread_id);
    bool IsValidData(std::string_view data, int64_t client_id) const;

    boost::asio::io_service io_context_;
    udp::socket socket_;
    udp::socket reg_socket_;

    std::unordered_map<int64_t, udp::endpoint> connections_;
    // std::unordered_map<udp::endpoint, int64_t> id_by_connection_;
    std::unordered_map<int64_t, DataQueue> users_data_;
    std::mt19937_64 rand_gen_;

    size_t user_counter_ = 0;
    std::unordered_map<int64_t, udp::endpoint> actual_connections_;
    std::unordered_map<int64_t, std::string> actual_message_;
};