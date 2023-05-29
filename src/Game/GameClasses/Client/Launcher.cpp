#include <sys/wait.h>
#include <iostream>
#include <NormInfrastructure/Client/Agent.h>
#include <cstddef>
#include "GameStarters/GameStarter.h"

enum Controls {
    Manual, Bot
};

int main(int argc, char* argv[]) {
    Agent& agent = Agent::GetInstance();
    uint64_t id = agent.GetUserID();
    std::cout << "Successfully connected to the game server with id " <<  id << std::endl;
    Controls controls = Manual;
    std::string path;
    while (true) {
        std::cout << "1 -> change input mode, 0 -> proceed with current" << std::endl;
        int i;
        std::cin >> i;
        if (i == 1) {
            if (controls == Controls::Manual) {
                controls = Controls::Bot;
                std::cout << "Enter path to the bot" << std::endl;
                std::cin >> path;
            } else {
                controls = Controls::Manual;
            }
        }
        std::cout << "Choose your character: " << std::endl;
        std::cout << "1 -> Mage, 2 -> Tank, 3 -> Pirate" << std::endl;
        Character character;
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1: {
                character = Character::MAGE;
                break;
            }
            case 2: {
                character = Character::TANK;
                break;
            }
            case 3: {
                character = Character::PIRATE;
                break;
            }
        }
        std::cout << "0 -> exit, 1 -> Create new game, 2 -> Connect to a given game, 3 -> Connect to a random game" << std::endl;
        std::cin >> choice;
        if (!choice) {
            break;
        }
        switch (choice) {
            case 1: {
                std::cout << "You've just created a lobby with id " << agent.CreateGame(character) << std::endl;
                break;
            }
            case 2: {
                uint64_t game_id;
                std::cout << "Enter id of the game you want to enter" << std::endl;
                std::cin >> game_id;
                agent.JoinGame(character, game_id);
                break;
            }
            case 3: {
                agent.JoinGame(character, 0);
                break;
            }
        }
        std::cout << "Waiting for the game to begin..." << std::endl;
        while (true) {
            if (agent.ApproveGame()) {
                std::cout << "Game starts!" << std::endl;
                pid_t pid = fork();
                if (pid) {
                    waitpid(pid, NULL, 0);
                } else {
                    if (controls == Controls::Manual) {
                        GameStarter::StartManualGame(agent.GetPort(), id);
                    } else {
                        GameStarter::StartBotGame(agent.GetPort(), id, path);
                    }
                }
                break;
            }
        }
    }
    return 0;
}