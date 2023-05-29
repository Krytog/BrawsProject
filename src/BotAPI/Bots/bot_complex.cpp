#include <iostream>
#include <random>
#include <vector>
#include "../../Game/GameClasses/GameObjectTags.h"

std::vector<char> moves = {'W', 'A', 'S', 'D'};

struct Object {
    std::string name;
    double pos_x;
    double pos_y;
};

int main() {
    bool attacked = false;
    while (true) {
        std::string server_info, output;

        std::vector<Object> to_attack;
        std::vector<Object> attacks;
        std::vector<Object> boosts;
        std::vector<Object> zones;
        Object myself;

        std::cin >> server_info;

        size_t left = server_info.find('{');
        size_t first_tag = server_info.find('#');
        size_t number_of_objects = std::stoull(server_info.substr(0, first_tag));  // No of objects
        size_t player_index = std::stoull(server_info.substr(first_tag + 1, left));  // Player number among them

        bool target_found = false;

        for (int i = 0; i < number_of_objects; ++i) {  // Get each object
            size_t right = server_info.find('}', left);
            size_t hashtag = server_info.find('#', left);
            size_t delim = server_info.find(';', left);

            std::string name = server_info.substr(left + 1, hashtag - left - 1);
            double pos_x = std::stod(server_info.substr(hashtag + 1, delim - hashtag - 1));
            double pos_y = std::stod(server_info.substr(delim + 1, right - delim - 1));

            Object object{.name = name, .pos_x = pos_x, .pos_y = pos_y};

            if (i == player_index) { myself = object; }
            else if (name == TAGS_CHARACTER_Bully || name == TAGS_CHARACTER_Pirate || name == TAGS_CHARACTER_Mage){
                to_attack.push_back(object);
            } else if (name == TAGS_POWERUP_DAMAGE || name == TAGS_POWERUP_HEALTH || name == TAGS_POWERUP_SPEED) {
                boosts.push_back(object);
            } else if (name == TAGS_PROJECTILE_MAGE || name == TAGS_PROJECTILE_BULLY || name == TAGS_PROJECTILE_PIRATE) {
                attacks.push_back(object);
            } else if (name == TAGS_ZONES_HAZARD) {
                zones.push_back(object);
            }

            left = right + 1;
        }

        // Got all the objects, now parse them

        Object o_to_att;
        if (!to_attack.empty() && !attacked) {  // attack someone
            attacked = true;
            o_to_att = to_attack[rand() % to_attack.size()];
            output += ("Y(" + std::to_string(o_to_att.pos_x) + ";" + std::to_string(o_to_att.pos_y) + ")#");
        } else {  // do not attack at all
            attacked = false;
            output += "N#";
        }

        if (attacked) {  // maniac mode
            double diff_x = o_to_att.pos_x - myself.pos_x;
            double diff_y = o_to_att.pos_y - myself.pos_y;
            if (fabs(diff_x) < 100.0) {}
            else if (diff_x > 0) {
                output += "D";
            } else {
                output += "A";
            }
            if (fabs(diff_y) < 100.0) {}
            else if (diff_y > 0) {
                output += "W";
            } else {
                output += "S";
            }
        } else {
            if (!attacks.empty()) {  // maneuver
                std::min_element(attacks.begin(), attacks.end(), [&myself](const Object& object1, const Object object2){
                    return (myself.pos_x - object1.pos_x) * (myself.pos_x - object1.pos_x) + (myself.pos_y - object1.pos_y) * (myself.pos_y - object1.pos_y) <
                           (myself.pos_x - object2.pos_x) * (myself.pos_x - object2.pos_x) + (myself.pos_y - object2.pos_y) * (myself.pos_y - object2.pos_y);
                });
                auto closest_attack = attacks.front();
                double diff_x = closest_attack.pos_x - myself.pos_x;
                double diff_y = closest_attack.pos_y - myself.pos_y;

                if (diff_x > 0) {
                    output += "A";
                } else {
                    output += "D";
                }
                if (diff_y > 0) {
                    output += "S";
                } else {
                    output += "W";
                }

            } else if (!zones.empty()) {  // run from zone
                std::min_element(zones.begin(), zones.end(), [&myself](const Object& object1, const Object object2){
                    return (myself.pos_x - object1.pos_x) * (myself.pos_x - object1.pos_x) + (myself.pos_y - object1.pos_y) * (myself.pos_y - object1.pos_y) <
                           (myself.pos_x - object2.pos_x) * (myself.pos_x - object2.pos_x) + (myself.pos_y - object2.pos_y) * (myself.pos_y - object2.pos_y);
                });
                auto closest_zone = zones.front();
                double diff_x = closest_zone.pos_x - myself.pos_x;
                double diff_y = closest_zone.pos_y - myself.pos_y;

                if (diff_x > 0) {
                    output += "A";
                } else {
                    output += "D";
                }
                if (diff_y > 0) {
                    output += "S";
                } else {
                    output += "W";
                }

            } else if (!boosts.empty()) {  // go for boosts
                std::min_element(boosts.begin(), boosts.end(), [&myself](const Object& object1, const Object object2){
                    return (myself.pos_x - object1.pos_x) * (myself.pos_x - object1.pos_x) + (myself.pos_y - object1.pos_y) * (myself.pos_y - object1.pos_y) <
                           (myself.pos_x - object2.pos_x) * (myself.pos_x - object2.pos_x) + (myself.pos_y - object2.pos_y) * (myself.pos_y - object2.pos_y);
                });
                auto closest_boost = boosts.front();
                double diff_x = closest_boost.pos_x - myself.pos_x;
                double diff_y = closest_boost.pos_y - myself.pos_y;

                if (diff_x > 0) {
                    output += "D";
                } else {
                    output += "A";
                }
                if (diff_y > 0) {
                    output += "W";
                } else {
                    output += "S";
                }

            } else {  // run somewhere
                output += moves[rand() % 4];
            }
        }

//        std::cerr << output << std::endl;
        std::cout << output << std::endl;
    }
}