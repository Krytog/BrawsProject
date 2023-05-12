#include "VisibleObject.h"
#include <cmath>

IVisibleObject::IVisibleObject(const Position& position, const double& angle, const RenderLayers& layer):
    pos_(position), angle_(angle), layer_(layer) {}

void IVisibleObject::Translate(const Vector2D &vector2d) {
    pos_.Translate(vector2d);
}

void IVisibleObject::UpdatePosition(const Position &position) {
    pos_ = position;
}

IVisibleObject::RenderLayers IVisibleObject::GetLayer() const {
    return layer_;
}

void IVisibleObject::UpdateRotation(const double& angle) {
    angle_ = angle;
}

void IVisibleObject::Rotate(const double& angle) {
    angle_ += angle;
}

double IVisibleObject::GetRotationAngle() const {
    return angle_;
}

Vector2D IVisibleObject::GetRotator() const {
    return Vector2D(std::cos(angle_), std::sin(angle_));
}

void IVisibleObject::UpdateRotation(const Vector2D &rotator) {
    const double sin_of_angle = rotator.GetCoordinates().second / rotator.Length();
    angle_ = std::asin(sin_of_angle);
}

void IFlexibleVisibleObject::SetDefaultLogic() {
    render_logic_ = nullptr;
}

void IFlexibleVisibleObject::RenderIt(Painter *painter) {
    if (render_logic_) {
        render_logic_(painter);
        return;
    }
    RenderItCustom(painter);
}

IFlexibleVisibleObject::IFlexibleVisibleObject(
    const Position& position,
    const double& angle,
    const RenderLayers& layer
): IVisibleObject(position, angle, layer) {};

