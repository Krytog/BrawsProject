#include "KripProducer.h"

#define RUN "run"
#define SHEET "sheet"

KripProducer::KripProducer(std::unique_ptr<Position>& pos_ptr, std::unique_ptr<Collider>& coll_ptr,
                           std::unique_ptr<VisibleObject>& vis_ptr, std::string_view tag,
                           const GameObject* obj_to_follow, GameMode mode)
    : GameObject(pos_ptr, coll_ptr, vis_ptr, tag), obj_to_follow_(obj_to_follow){};

void KripProducer::OnUpdate() {
    ProduceKrip();
}

void KripProducer::ProduceKrip() {
    std::cout << "ProduceKripStart\n";
    if (engine_->GetTicksCount() % spawn_frequency_) {
        return;
    }
    std::vector<Position> positions =
        random_.RandPoints(obj_to_follow_->GetPosition(), spawn_radius_, number_per_spawn_);
    for (const auto& pos : positions) {
        //        std::vector<std::pair<std::string_view, VisibleObject*>> decoy_vector;
        auto officer_animation =
            new AnimatedSprite(new Position(pos), 100, 100, "../Game/DEMOs/v0.3/Resources/ZZZ.png",
                               LEVELS::FIRST_USER_LEVEL, 4, 2, 3, {}, true);
        //        auto blow_animation = new AnimatedSprite(new Position(pos), 100, 100,
        //        "../Game/DEMOs/v0.3/Resources/boom.png", LEVELS::FIRST_USER_LEVEL, 4, 2, 2, {}, false);
        //        decoy_vector.emplace_back(RUN, officer_animation);
        //        decoy_vector.emplace_back(SHEET, blow_animation);

        engine_->ProduceObject<Krip>(new Position(pos), new RectangleCollider(Position(pos), 100, 100),
                                     officer_animation, "officer", CharacterInitArgPack{}, speed_,
                                     &deaths_counter_, obj_to_follow_);
    }
    std::cout << "ProduceKripEnd\n";
}

Random::Random() : gen_(rd_()), distrib_(std::uniform_int_distribution<>(-dist_, dist_)){};

double Random::GetRandUnitNumber() {
    return static_cast<double>(distrib_(gen_)) / dist_;
}

std::vector<Position> Random::RandPoints(const Position& center_pos, double radius, size_t number) {
    std::cout << "RandStart\n";
    std::vector<Position> result;
    for (size_t it = 0; it < number; it++) {
        double theta = 2 * M_PI * GetRandUnitNumber();
        result.push_back({center_pos.GetCoordinates().first + radius * cos(theta),
                          center_pos.GetCoordinates().second + radius * sin(theta)});
    }
    std::cout << "RandEnd\n";
    return result;
}
