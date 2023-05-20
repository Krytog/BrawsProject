#pragma once

#include <Core/ServerEngine.h>
#include <Core/MyTime.h>

class DamageZonePawnServer : public GameObject {
public:
    DamageZonePawnServer(double damage, const Position& center, double width, double height);
    ~DamageZonePawnServer();

    double GetWidth() const;
    double GetHeight() const;

    void OnUpdate() override;

    static const size_t kTypeId;

private:
    double damage_;
    double width_;
    double height_;
    MyTime timer;

    size_t cerebrate_id_;
};
