#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <random>
#include <netinet/in.h>

namespace {
    char client_port[] = "31337";
    const int kMaxDtgrmLen = 1024;
}

using SockaddrInfo = std::pair<std::unique_ptr<struct sockaddr_in>, socklen_t>;

class Communicator {
public:
    static Communicator &GetInstance();

    // rewrite with epoll
    int64_t RegUser();

    // rewrite with epoll
    std::string ReceiveFromClient(uint64_t client_id);
    void SendToClient(uint64_t client_id, std::string_view data);

private:
    Communicator();
    Communicator(const Communicator &other) = delete;
    Communicator(Communicator &&other) = delete;

    Communicator &operator=(const Communicator &other) = delete;
    Communicator &operator=(Communicator &&other) = delete;

    ~Communicator();

    uint64_t GenId();

    int sock_fd_;
    std::unordered_map<int64_t, SockaddrInfo> register_;
    std::unordered_map<int64_t, std::string> message_log_;

    std::unordered_set<uint64_t> ids_;
    std::mt19937_64 rand_gen_;
};