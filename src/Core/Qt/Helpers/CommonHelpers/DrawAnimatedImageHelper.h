#pragma once

#include <qimage.h>
#include "../../../Position.h"
#include "../BasicHelper.h"
#include"../../PaintTemplates/AnimatedSprite.h"
#include <memory>

class DrawAnimatedImageHelper : public IBasicHelper {
public:
    DrawAnimatedImageHelper(AnimatedSprite* image, const Position&, const size_t& clockwise_angle = 0);

    virtual void Paint(QPainter* painter) const override;
private:
    AnimatedSprite* image_;
    Position pos_;
    size_t angle_;
};