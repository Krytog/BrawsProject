#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <random>
#include <boost/asio.hpp>
#include <deque>
#include <cstdint>
#include <iostream>
#include <thread>

using boost::asio::ip::udp;

enum Character {
    MAGE,
    TANK,
    PIRATE
};

struct Player {
    uint64_t id;
    udp::endpoint connection;
    Character character;
};

class Porter {
private:
    class Lobby {
    public:
        enum Status : uint8_t {
            Waiting,
            Running,
            Finished
        };

        Lobby(size_t users_count);

        void SetPlayerCount(size_t users_count);
        void AddPlayer(const Player& player);
        bool RemovePlayer(uint64_t id);
        bool Ready() const;
        void Clear();

        Status GetStatus() const;
        void SetStatus(Status status);

        ~Lobby();
    private:
        Status status_;
        std::unordered_map<uint64_t, Player> players_;
        size_t users_count_;
    };

public:
    static Porter &GetInstance();

    uint64_t RegUser();
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
    std::unordered_map<uint64_t, Lobby> lobbies_;
    boost::asio::io_service io_context_;
    udp::socket reg_socket_;
    std::unordered_map<uint64_t, udp::endpoint> connections_;

    // ID randomizer
    std::random_device rd_;
    std::mt19937_64 gen_;
    std::uniform_int_distribution<uint64_t> dis_;

    size_t user_counter_ = 0;
};