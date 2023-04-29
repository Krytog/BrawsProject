#include "VisibleObject.h"

IVisibleObject::IVisibleObject(const Position& position):
    pos_(position) {}

void IVisibleObject::Translate(const Vector2D &vector2d) {
    pos_.Translate(vector2d);
}

void IVisibleObject::UpdatePosition(const Position &position) {
    pos_ = position;
}

void IFlexibleVisibleObject::SetDefaultLogic() {
    render_logic_ = nullptr;
}

bool IFlexibleVisibleObject::IsCustomRendering() const {
    if (render_logic_) {
        return true;
    }
    return false;
}

void IFlexibleVisibleObject::RenderItCustom(const QPainter* painter) const {
    render_logic_(painter);
}