#include "RotateHelper.h"

RotateHelper::RotateHelper(Painter *painter, const double& clockwise_angle): angle_(clockwise_angle) {
    painter_ = dynamic_cast<QPainter*>(painter);
    painter_->save();
}

void RotateHelper::Paint(Painter *painter) const {
    painter_->rotate(angle_);
}

RotateHelper::~RotateHelper() {
    painter_->restore();
}