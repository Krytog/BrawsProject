#pragma once

#include <string_view>
#include <string>
#include <unordered_map>
#include <memory>
#include <boost/asio.hpp>
#include <deque>

using boost::asio::ip::udp;

class Communicator {
public:
    static Communicator &GetInstance(uint16_t port);

    std::string ReceiveFromServer();
    void SendToServer(std::string_view data);
    void BindOnPort(uint16_t port);
    void SetId(uint64_t  id);

    void Run();
    void Stop();

    ~Communicator() = default;
private:
    Communicator(uint16_t port);

    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    std::string package_;

    uint64_t user_id_;

    boost::asio::io_context io_context_;
    udp::socket socket_;
    std::thread accept_thread_;

    udp::resolver::results_type endpoints_;

    udp::endpoint connection_;

private:
    void DoReceive();
};