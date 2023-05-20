#pragma once

#include <_types/_uint16_t.h>
#include <_types/_uint64_t.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <random>
#include <boost/asio.hpp>
#include <deque>
#include <cstdint>
#include <iostream>
#include <thread>
#include <atomic>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

enum RequestType: uint32_t {
    ConnectToGame = 1,
    CreateNewGame = 2
};

enum Character {
    MAGE,
    TANK,
    PIRATE
};

struct Player {
    uint64_t id;
    udp::endpoint endpoint;
    Character character;
};

struct Request {
    RequestType type;
    uint64_t id;
    Character character_type;
};

struct LobbySettings {
    uint64_t users_count;
};

class Porter {
private:
    class Lobby {
    public:
        enum Status : uint8_t {
            Waiting,
            IsReady,
            Running,
            Finished
        };

        Lobby(size_t users_count);

        void SetPlayerCount(size_t users_count);
        void AddPlayer(Player player);
        bool RemovePlayer(uint64_t id);
        void Clear();

        Status GetStatus() const;
        void SetStatus(Status status);

    private:
        Status status_;
        std::unordered_map<uint64_t, Player> players_;
        size_t users_count_;
        size_t registered_ = 0;
    };

public:
    static constexpr uint16_t kMaxPackageLen = 3200;

public:
    static Porter &GetInstance();

    void StartRegistration();
    void CheckLobbiesState();

    ~Porter() = default;
private:
    Porter();

    Porter(const Porter &other) = delete;
    Porter(Porter &&other) = delete;

    Porter &operator=(const Porter &other) = delete;
    Porter &operator=(Porter &&other) = delete;

    uint64_t RegId();
    void RegUser();
    uint64_t RegLobbyId();

private:
    std::unordered_map<uint64_t, Lobby> lobbies_; 
    boost::asio::io_service io_context_;
    tcp::acceptor acceptor_;
    std::unordered_map<uint64_t, tcp::socket> connections_;

    // ID randomizer
    std::random_device rd_;
    std::mt19937_64 gen_;
    std::uniform_int_distribution<uint64_t> dis_;

    //Synchrone
    std::mutex wait_reg_;
    std::atomic<bool> has_incoming_users_ = false;
    std::thread reg_thread_;
};