#include "VisibleObject.h"

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

void IVisibleObject::UpdateAngle(const double& angle) {
    angle_ = angle;
}

void IVisibleObject::TranslateOnAngle(const double& angle) {
    angle_ += angle;
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

