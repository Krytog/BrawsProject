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

    uint64_t RegUser();
    std::string ReceiveFromClient(uint64_t client_id);
    void SendToClient(uint64_t client_id, std::string_view data);
    void RunFor(size_t milliseconds);
    void Run();
    void Stop();
    size_t GetUserNumber();

    ~Communicator() = default;
private:
    Communicator();

    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    uint64_t RegId();
    void DoReceive(uint64_t thread_id);
    bool IsValidData(std::string_view data, uint64_t client_id) const;

    boost::asio::io_context io_context_;
    udp::socket socket_;
    udp::socket reg_socket_;

    std::unordered_map<uint64_t, udp::endpoint> connections_;
    std::unordered_map<uint64_t, FixedQueue<std::string, kQueueSize>> users_data_;

    // ID randomizer
    std::random_device rd_;
    std::mt19937_64 gen_;
    std::uniform_int_distribution<uint64_t> dis_;

    size_t user_counter_ = 0;
    std::unordered_map<uint64_t, udp::endpoint> actual_connections_;
    std::unordered_map<uint64_t, std::string> actual_message_;
    std::thread accept_thread_;
};