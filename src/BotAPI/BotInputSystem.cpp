#include <iostream>
#include <cassert>

#include "BotInputSystem.h"


BotInputSystem::BotInputSystem(const sf::Window& window) : InputSystem(window) {}

BotInputSystem& BotInputSystem::InitInstance(const sf::Window& window) {
    static BotInputSystem instance(window);
    return instance;
}

// N/Y(PosX;PosY)#WASDQE

void BotInputSystem::ReadNewInput() {
    std::string command;
    std::cin >> command;  // command line sent from bot

    input_tokens_.clear();

    size_t hashtag = 0;
    if (command[0] == 'N') {
        input_tokens_.emplace_back(MouseToken{0, Position()});
        hashtag = 1;
    } else {
        double x, y;
        // command[1] == '('

        size_t first = 2, last = 2;
        while (command[last] != ';') {
            ++last;
        }
        x = std::stod(command.substr(first, last - first));

        first = last;
        while (command[last] != ')') {
            ++last;
        }
        y = std::stod(command.substr(first, last - first));

        input_tokens_.emplace_back(MouseToken{1, Position(x, y)});
        hashtag = last + 1;
    }
    for (size_t i = hashtag + 1; i < command.size(); ++i) {
        // check that symbol is in {WASDQE}
        input_tokens_.emplace_back(KeyboardToken{command[i]});
    }
}
