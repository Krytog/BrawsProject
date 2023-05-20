#pragma once

#include <Core/ServerEngine.h>
#include <Core/MyTime.h>

class HazardZoneManager : public GameObject {
public:
    HazardZoneManager(double width, double height, double total_time, double tick_time, double damage);

    void OnUpdate() override;
private:
    GameObject* left_zone_;
    GameObject* right_zone_;
    GameObject* up_zone_;
    GameObject* down_zone_;

    MyTime timer_;
    uint64_t ticks_;
    double per_tick_;

    double width_;
    double height_;

    double damage_;

    double DiscreteTime() const;
    void ManageZones();
    void ManageLeftZone();
    void ManageRightZone();
    void ManageUpZone();
    void ManageDownZone();
};