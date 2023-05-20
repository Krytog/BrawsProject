#pragma once

#include <Core/VisibleGameObject.h>

class DamageZonePawnClient : public PersistentVisibleObject {
public:
    DamageZonePawnClient();
    ~DamageZonePawnClient();

    void UpdateSize(double width, double height);

    static const size_t kTypeId;

private:
    double width_;
    double height_;
};
