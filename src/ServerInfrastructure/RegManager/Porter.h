#pragma once

#include <_types/_uint64_t.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <random>
#include <boost/asio.hpp>
#include <deque>
#include <thread>
#include "../Lobby/Lobby.h"

using boost::asio::ip::udp;

class Porter {
    static constexpr uint16_t kMaxDtgrmLen = 3200;
public:
    static Porter &GetInstance();

    uint64_t RegUser();
    std::string ReceiveFromClient(uint64_t client_id);
    void SendToClient(uint64_t client_id, std::string_view data);
    void Run();

    ~Porter() = default;
private:
    Porter();

    Porter(const Porter &other) = delete;
    Porter(Porter &&other) = delete;

    Porter &operator=(const Porter &other) = delete;
    Porter &operator=(Porter &&other) = delete;

    uint64_t RegId();
    void DoRecieve(size_t thread_id);


private:
    std::unordered_map<uint64_t, > 
    boost::asio::io_service io_context_;
    udp::socket reg_socket_;
    std::unordered_map<uint64_t, udp::endpoint> connections_;

    // ID randomizer
    std::random_device rd_;
    std::mt19937_64 gen_;
    std::uniform_int_distribution<uint64_t> dis_;

    size_t user_counter_ = 0;
    std::unordered_map<uint64_t, udp::endpoint> actual_connections_;
    std::unordered_map<uint64_t, std::string> actual_message_;
    std::unique_ptr<std::thread> accept_thread_;
};