#include <iostream>
#include <random>
#include <vector>
#include "../../Game/GameClasses/GameObjectTags.h"

std::vector<char> moves = {'W', 'A', 'S', 'D'};

int main() {
    while (true) {
        std::string server_info, output;
        std::cin >> server_info;

        size_t left = server_info.find('{');
        size_t first_tag = server_info.find('#');
        size_t number_of_objects = std::stoull(server_info.substr(0, first_tag));
        size_t player_index = std::stoull(server_info.substr(first_tag + 1, left));

        bool target_found = false;

        for (int i = 0; i < number_of_objects; ++i) {  // Parse each object
            size_t right = server_info.find('}', left);
            size_t hashtag = server_info.find('#', left);
            size_t delim = server_info.find(';', left);

            std::string name = server_info.substr(left + 1, hashtag - left - 1);
            std::string pos_x = server_info.substr(hashtag + 1, delim - hashtag - 1);
            std::string pos_y = server_info.substr(delim + 1, right - delim - 1);
//            std::cerr << name << " " << pos_x << " " << pos_y << std::endl;  // seems to work
            if (name == TAGS_CHARACTER_Bully) {
                target_found = true;
                output += ("Y(" + pos_x + ";" + pos_y + ")#");
            }

            left = right + 1;
        }
        if (!target_found) {
            output += "N#";
        }
        if (rand() % 2 == 0) {
            output = "N#";
        }
        std::cerr << output << std::endl;
        std::cout << output << std::endl;
    }
}