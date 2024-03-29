#pragma once

#include <string_view>
#include "../BasicHelper.h"
#include "../../PaintTemplates/TextSprite.h"

class DrawTextHelper final: public IBasicHelper {
public:
    DrawTextHelper(TextSprite* text, Position pos, const size_t& clockwise_angle = 0);
    void Paint(Painter *painter) const override;

private:
    Position pos_;
    TextSprite* text_;
    size_t angle_;
};