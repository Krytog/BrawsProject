#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <random>
#include <deque>
#include <queue>
#include <thread>

#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include "../GameInfo.h"

using boost::asio::ip::udp;

class Communicator {
    static constexpr uint16_t kMaxDtgrmLen = 3200;
    static constexpr size_t kQueueSize = 5;

    template <typename T, size_t QueueSize>
    class FixedQueue : public std::queue<T, boost::circular_buffer<T>> {
    public:
        FixedQueue() : std::queue<T, boost::circular_buffer<T>>(boost::circular_buffer<T>(QueueSize)) {}
    };
public:
    static Communicator &GetInstance();

    std::string ReceiveFromClient(uint64_t client_id);
    void SendToClient(uint64_t client_id, std::string_view data);
    void SetClients(const std::unordered_map<uint64_t, Player>& players);
    void Run();
    void Stop();
    size_t GetUserNumber();

    ~Communicator();
private:
    Communicator();

    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    void DoReceive(uint64_t thread_id);
    bool IsValidData(std::string_view data, uint64_t client_id) const;

    boost::asio::io_context io_context_;
    udp::socket socket_;

    std::unordered_map<uint64_t, Player> players_;
    std::unordered_map<uint64_t, FixedQueue<std::string, kQueueSize>> users_data_;

    // ID randomizer
    std::random_device rd_;
    std::mt19937_64 gen_;
    std::uniform_int_distribution<uint64_t> dis_;

    // Requests handling
    size_t user_counter_ = 0;
    std::unordered_map<uint64_t, udp::endpoint> actual_connections_;
    std::unordered_map<uint64_t, std::string> actual_message_;
    std::thread accept_thread_;
};