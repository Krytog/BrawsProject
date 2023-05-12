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
    Communicator(boost::asio::io_context& io_context);
    // static Communicator &GetInstance();

    int64_t RegOnServer();

    std::string ReceiveFromServer();
    void SendToServer(std::string_view data);

    ~Communicator() {
        io_context_.run();
    };
private:
    // Communicator();

    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    std::deque<std::string> packages_;

    int64_t user_id_;

    udp::socket socket_;

    udp::resolver::results_type endpoints_;
    udp::resolver::results_type reg_endpoints_;

    boost::asio::io_context io_context_;
    udp::endpoint connection_;

private:
    void DoRecieve();
};