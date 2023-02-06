#pragma once

#include <random>
#include <math.h>

#include "../../Core/CustomBehaviour.h"
#include "Krip.h"

// Need to remove to Tools Later
class Random {
public:
    Random();
    std::vector<Position> RandPoints(const Position& center_pos, double radius, size_t number);

private:
    double GetRandUnitNumber();

    size_t dist_ = 1000000;
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_int_distribution<> distrib_;
};

enum GameMode {
    EASY,
    MEDIUM,
    HARD,
    GOD /* Are you Zelensky? */
};

class KripProducer: public virtual CustomBehaviour, public virtual GameObject {
public:
    KripProducer(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag,
                 const GameObject* obj_to_follow, GameMode mode);

    KripProducer(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag,
                 const GameObject* obj_to_follow, const double& krip_start_health, const double& krip_damage, const double& krip_speed, const double& krip_spawn_radius,
                 const double& krip_number_per_spawn_, GameMode mode);

    void OnUpdate() override;
    void ProduceKrip();

private:
    const GameObject* obj_to_follow_;
    size_t deaths_counter_;
    Random random_;

    // Krip's characteristics
    double start_health_ = 50;
    double damage_ = 20;
    double speed_ = 7.5;

    double spawn_radius_ = 800;
    size_t spawn_frequency_ = 500;
    size_t number_per_spawn_ = 2;
};