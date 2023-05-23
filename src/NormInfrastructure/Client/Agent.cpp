#include "Agent.h"

Agent& Agent::GetInstance() {
    static Agent instance;
    return instance;
}

Agent::Agent() : socket_(io_context_) {
    // Establish connection
    tcp::resolver resolver(io_context_);
    tcp::resolver::query query(tcp::v4(), GAME_HOST, STR(GAME_PORT));
    tcp::resolver::iterator iterator = resolver.resolve(query);
    boost::asio::connect(socket_, iterator);

    // Get ID from Server
    Read(&player_id_);
}

uint64_t Agent::CreateGame(Character character, const GameSettings& settings) {
    Request request{.type = RequestType::CreateNewGame, .id = 0, .character_type = character};
    Write(&request);
    Write(&settings);
    uint64_t game_id;
    Read(&game_id);
    return game_id;

}
void Agent::JoinGame(Character character, uint64_t game_id) {
    Request request{.type = RequestType::ConnectToGame, .id = game_id, .character_type = character};
    Write(&request);
}

void Agent::LeaveGame() {
    Request request{.type = RequestType::LeaveGame};
    Write(&request);
}

uint64_t Agent::GetUserID() {
    return player_id_;
}

bool Agent::ApproveGame() {
    boost::system::error_code error;
    socket_.non_blocking(true);
    char message[strlen(GAME_APPROVE)];
    boost::asio::read(socket_, boost::asio::buffer(message, sizeof(message)), error);
    if (error == boost::asio::error::eof) {
        return false;
    }
    socket_.non_blocking(false);
    return true;
}


