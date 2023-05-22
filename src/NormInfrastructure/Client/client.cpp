#include <iostream>
#include <boost/asio.hpp>
#include "Agent.h"

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[]) {
    Agent& agent = Agent::GetInstance();
    uint64_t id = agent.GetUserID();

    std::cout << "Our ID " <<  id << std::endl;
    std::cout << "Lobby ID " << agent.CreateGame(Character::PIRATE) << std::endl;
//    agent.LeaveGame();
    std::cout << "Lobby ID " << agent.CreateGame(Character::PIRATE) << std::endl;
    return 0;
}