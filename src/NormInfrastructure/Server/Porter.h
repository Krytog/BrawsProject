#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <cstdint>
#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>

#include <boost/asio.hpp>
#include "../GameInfo.h"

void Game(const std::unordered_map<uint64_t, Player>&);

class Porter {
private:
    class Lobby {
    public:
        enum Status : uint8_t {
            Waiting,
            Running,
            Finished
        };

        Lobby(uint64_t users_count);
        Lobby(const GameSettings& settings);

        void SetPlayerCount(uint64_t users_count);
        void AddPlayer(Player player);
        bool RemovePlayer(uint64_t id);
        bool Ready() const;
        void Clear();

        Status GetStatus() const;
        void SetStatus(Status status);
        std::unordered_map<uint64_t, Player> GetPlayers() const;

    private:
        Status status_ = Waiting;
        std::unordered_map<uint64_t, Player> players_;
        uint64_t users_count_;
    };

public:
    static constexpr uint16_t kMaxPackageLen = 3200;

public:
    static Porter &GetInstance();

    void StartRegistration();
    void StartHandling();
    void SendInitGamePackages(const Lobby& lobby);


    void CheckLobbiesState();
    void InitGame(Lobby& lobby);

    ~Porter();
private:
    Porter();

    Porter(const Porter &other) = delete;
    Porter(Porter &&other) = delete;

    Porter &operator=(const Porter &other) = delete;
    Porter &operator=(Porter &&other) = delete;

    uint64_t RegId();
    uint64_t RegLobbyId();

    void RegUser();
    void HandleRequests();
    void HandleConnection(uint64_t user_id, tcp::socket& connection, const Request& request);

private:
    std::unordered_map<uint64_t, Lobby> lobbies_;  // lobby ID -> lobby
    std::unordered_map<uint64_t, uint64_t> players_;   // player ID -> player's lobby ID
    boost::asio::io_context io_context_;
    tcp::acceptor acceptor_;

    std::unordered_map<uint64_t, tcp::socket> connections_;
    std::unordered_map<uint64_t, uint16_t> ports_;

    // ID randomizer
    std::random_device rd_;
    std::mt19937_64 gen_;
    std::uniform_int_distribution<uint64_t> dis_;

    // Synchronization
    std::mutex lobby_lock_;
    std::mutex reg_mutex_;

    std::atomic<bool> has_incoming_users_ = false;
    std::unordered_map<uint64_t, tcp::socket> incoming_users_;
    std::unordered_set<uint64_t> used_user_ids_;

    std::thread accept_thread_;
    std::thread reg_thread_;
};