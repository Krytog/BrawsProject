#pragma once

#include "../BasicHelper.h"
#include <QPainter>

class RotateHelper final: public IBasicHelper {
public:
    RotateHelper(Painter *painter, const double& clockwise_angle = 0);
    void Paint(Painter *painter) const override;
    ~RotateHelper();

private:
    double angle_;
    QPainter* painter_;
};