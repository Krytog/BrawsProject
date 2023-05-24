#include <boost/asio.hpp>
#include <random>
#include "../GameInfo.h"

void Game();

class Agent {
public:
    static Agent &GetInstance();

    // Returns lobby ID
    uint64_t CreateGame(Character character, const GameSettings& settings = {.users_count = 2});
    void JoinGame(Character character, uint64_t game_id = 0); // id == 0 <=> join any game
    void LeaveGame();
    uint16_t GetPort();

    bool ApproveGame();

    // Returns user's ID
    uint64_t GetUserID();

private:
    template <typename Buf>
    std::size_t Read(Buf* buffer) {
        return boost::asio::read(socket_, boost::asio::buffer(buffer, sizeof(*buffer)));
    }
    template <typename Buf>
    std::size_t Write(Buf* buffer) {
        return boost::asio::write(socket_, boost::asio::buffer(buffer, sizeof(*buffer)));
    }
    void EndGameSession();

    Agent();
    ~Agent();

    Agent(const Agent &other) = delete;
    Agent(Agent &&other) = delete;

    Agent &operator=(const Agent &other) = delete;
    Agent &operator=(Agent &&other) = delete;

    boost::asio::io_context io_context_;
    tcp::socket socket_;
    uint64_t player_id_;
    uint16_t port_;

    // Random
    std::random_device rd_;
    std::mt19937_64 gen_;
    std::uniform_int_distribution<uint64_t> dis_;
};