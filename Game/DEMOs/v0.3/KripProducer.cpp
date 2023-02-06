#include "KripProducer.h"

#define RUN "run"
#define SHEET "sheet"

KripProducer::KripProducer(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr,
                           std::string_view tag, const GameObject* obj_to_follow, GameMode mode): GameObject(pos_ptr, coll_ptr, vis_ptr, tag),
                            obj_to_follow_(obj_to_follow) {};

KripProducer::KripProducer(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr, std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag,
                           const GameObject* obj_to_follow, const double& krip_start_health, const double& krip_damage, const double& krip_speed, const double&
                                                                              krip_spawn_radius, const double& krip_number_per_spawn_, GameMode mode): GameObject(pos_ptr, coll_ptr, vis_ptr, tag),
                              obj_to_follow_(obj_to_follow), start_health_(krip_start_health), damage_(krip_damage), speed_(krip_speed), spawn_radius_(krip_spawn_radius), number_per_spawn_(krip_number_per_spawn_) {}

void KripProducer::OnUpdate() {
    ProduceKrip();
}

void KripProducer::ProduceKrip() {
    if (engine_->GetTicksCount() % spawn_frequency_) {
        return;
    }
    std::vector<Position> positions = random_.RandPoints(obj_to_follow_->GetPosition(), spawn_radius_, number_per_spawn_);
    for (const auto& pos: positions) {
        if (Krip::GetInstanceCount() < 10) {
            engine_->ProduceObject<Krip>(new Position(pos),
                                         new RectangleCollider(Position(pos), 40, 150, false),
                                         nullptr,
                                         "officer",
                                         speed_,
                                         &deaths_counter_,
                                         obj_to_follow_);
        }
    }
}

Random::Random(): gen_(rd_()), distrib_(std::uniform_int_distribution<>(-dist_, dist_)) {};

double Random::GetRandUnitNumber() {
    return static_cast<double>(distrib_(gen_)) / dist_;
}

std::vector<Position> Random::RandPoints(const Position& center_pos, double radius, size_t number) {
    std::vector<Position> result;
    for (size_t it = 0; it < number; it++) {
        double theta = 2 * M_PI * GetRandUnitNumber();
        result.push_back({ center_pos.GetCoordinates().first + radius * cos(theta),
                       center_pos.GetCoordinates().second + radius * sin(theta) });
    }
    return result;
}