#include <iostream>
#include <random>
#include <vector>

std::vector<char> moves = {'W', 'A', 'S', 'D'};

int main() {
    std::string server_info, output;
    while (true) {
        output = "N#";
        output += moves[rand() % 4];
        for (int i = 0; i < 12; ++i) {
            // Get Info
            std::cin >> server_info;

            // Parse
            /*
             * This bot is too primitive to parse
             * */

            // Send
            std::cout << output << std::endl;
        }
    }
}