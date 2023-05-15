#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <random>
#include <boost/asio.hpp>
#include <deque>
#include <thread>

namespace {
    const int kMaxDtgrmLen = 32000;
}

using boost::asio::ip::udp;

class Communicator {
    using DataQueue = std::deque<std::string>;
public:
    static Communicator &GetInstance();

    uint64_t RegUser();
    std::string ReceiveFromClient(uint64_t client_id);
    void SendToClient(uint64_t client_id, std::string_view data);
    void RunFor(size_t milliseconds);
    void Run();

    ~Communicator() = default;
private:
    Communicator();

    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    uint64_t RegId();
    void DoRecieve(size_t thread_id);
    bool IsValidData(std::string_view data, uint64_t client_id) const;

    boost::asio::io_service io_context_;
    udp::socket socket_;
    udp::socket reg_socket_;

    std::unordered_map<uint64_t, udp::endpoint> connections_;
    std::unordered_map<uint64_t, DataQueue> users_data_;

    // ID randomizer
    std::random_device rd_;
    std::mt19937_64 gen_;
    std::uniform_int_distribution<uint64_t> dis_;

    size_t user_counter_ = 0;
    std::unordered_map<uint64_t, udp::endpoint> actual_connections_;
    std::unordered_map<uint64_t, std::string> actual_message_;
    std::unique_ptr<std::thread> accept_thread_;
};