#include <iostream>
#include <cassert>
#include <unordered_set>

#include "BotInputSystem.h"
#include "BotManagement.h"

BotInputSystem* BotInputSystem::InitInstance() {
    static BotInputSystem instance;
    return &instance;
}

// N/Y(PosX;PosY)#WASDQE

std::unordered_set<char> moves = {'W', 'A', 'S', 'D', 'Q', 'E'};

void BotInputSystem::ReadNewInput() {
    BotManagement::GetInstance().SendInfoToBot();  // send info to bot

    std::string command;
    std::cin >> command;  // command line sent from bot

    input_tokens_.clear();
    try {
        size_t hashtag = 0;
        if (command[0] == 'N') {
            input_tokens_.emplace_back(MouseToken{0, Position()});
            hashtag = 1;
        } else {
            double x, y;
            // command[1] == '('

            size_t first = 2, last = 2;
            while (last < command.length() && command[last] != ';') {
                ++last;
            }
            x = std::stod(command.substr(first, last - first));

            first = last + 1;
            while (last < command.length() && command[last] != ')') {
                ++last;
            }
            y = std::stod(command.substr(first, last - first));

            std::cerr << "Done" << std::endl;
            input_tokens_.emplace_back(MouseToken{1, Position(x, y)});
            hashtag = last + 1;
        }
        for (size_t i = hashtag + 1; i < command.size(); ++i) {
            // check that symbol is in {WASDQE}
            if (!moves.contains(command[i])) { throw std::runtime_error("Bad move"); }
            input_tokens_.emplace_back(KeyboardToken{command[i]});
        }
    } catch (...) {
        std::cerr << "Invalid Input!!!" << std::endl;
    }
}
