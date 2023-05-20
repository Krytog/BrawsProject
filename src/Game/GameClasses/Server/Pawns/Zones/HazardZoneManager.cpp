#include "HazardZoneManager.h"

#include "DamageZonePawnServer.h"
#include <cmath>

#include <Game/GameClasses/GameObjectTags.h>

HazardZoneManager::HazardZoneManager(double width, double height, double total_time, double tick_time, double damage):
    width_(width), height_(height), damage_(damage), per_tick_(tick_time),
    left_zone_(nullptr), right_zone_(nullptr), up_zone_(nullptr), down_zone_(nullptr) {
    ticks_ = std::ceil(total_time / tick_time) - 1;
    tag_ = TAGS_ZONES_HAZARDZONEMANAGER;
}

void HazardZoneManager::ManageLeftZone() {
    if (left_zone_) {
        ServerEngine::GetInstance().Destroy(left_zone_);
        left_zone_ = nullptr;
    }
    const double t = DiscreteTime();
    const double width = width_ * t / 2;
    const double height = height_;
    const double center_x = width_ * t / 4 - width_ / 2;
    const double center_y = 0;
    left_zone_ = ServerEngine::GetInstance().CreateGameObject<DamageZonePawnServer>(damage_, Position(center_x, center_y), width, height);
}

void HazardZoneManager::ManageRightZone() {
    if (right_zone_) {
        ServerEngine::GetInstance().Destroy(right_zone_);
        right_zone_ = nullptr;
    }
    const double t = DiscreteTime();
    const double width = width_ * t / 2;
    const double height = height_;
    const double center_x = width_ / 2 - width_ * t / 4;
    const double center_y = 0;
    right_zone_ = ServerEngine::GetInstance().CreateGameObject<DamageZonePawnServer>(damage_, Position(center_x, center_y), width, height);
}

void HazardZoneManager::ManageUpZone() {
    if (up_zone_) {
        ServerEngine::GetInstance().Destroy(up_zone_);
        up_zone_ = nullptr;
    }
    const double t = DiscreteTime();
    const double sides_width = t * width_;
    const double width = width_ - sides_width;
    const double height = height_ * t / 2;
    const double center_x = 0;
    const double center_y = height_ / 2 - height_ * t / 4;;
    up_zone_ = ServerEngine::GetInstance().CreateGameObject<DamageZonePawnServer>(damage_, Position(center_x, center_y), width, height);
}

void HazardZoneManager::ManageDownZone() {
    if (down_zone_) {
        ServerEngine::GetInstance().Destroy(down_zone_);
        down_zone_ = nullptr;
    }
    const double t = DiscreteTime();
    const double sides_width = t * width_;
    const double width = width_ - sides_width;
    const double height = height_ * t / 2;;
    const double center_x = 0;
    const double center_y = height_ * t / 4 - height_ / 2;
    down_zone_ = ServerEngine::GetInstance().CreateGameObject<DamageZonePawnServer>(damage_, Position(center_x, center_y), width, height);
}

void HazardZoneManager::ManageZones() {
    static double last_value = -1;
    const double time = DiscreteTime();
    if (last_value != time) {
        last_value = time;
        ManageLeftZone();
        ManageRightZone();
        ManageUpZone();
        ManageDownZone();
    }
}

double HazardZoneManager::DiscreteTime() const {
    const uint64_t cur_tick = timer_.EvaluateTime() / per_tick_;
    return double(cur_tick) / ticks_;
}

void HazardZoneManager::OnUpdate() {
    ManageZones();
}