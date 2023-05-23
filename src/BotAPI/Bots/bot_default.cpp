#include <iostream>
#include <random>
#include <vector>
#include "../../Game/GameClasses/GameObjectTags.h"

std::vector<char> moves = {'W', 'A', 'S', 'D'};

int main() {
    std::string server_info, output;
    while (true) {
        std::cin >> server_info;
        size_t left = server_info.find('{');
        size_t number_of_objects = std::stoull(server_info.substr(0, left));

        for (int i = 0; i < number_of_objects; ++i) {  // Parse each object
            size_t right = server_info.find('}', left);
            size_t hashtag = server_info.find('#', left);
            size_t delim = server_info.find(';', left);

            std::string name = server_info.substr(left + 1, hashtag - left - 1);
            std::string pos_x = server_info.substr(hashtag + 1, delim - hashtag - 1);
            std::string pos_y = server_info.substr(delim + 1, right - delim - 1);
//            std::cout << name << " " << pos_x << " " << pos_y << std::endl;  // seems to work
            if (name == TAGS_CHARACTER_Pirate) {
                output += ("Y(" + pos_x + ";" + pos_y + ")");
            }

            left = right + 1;
        }

        std::cout << output << std::endl;
    }
}