#pragma once

#include <qimage.h>
#include "../../../Position.h"
#include "../BasicHelper.h"
#include"../../PaintTemplates/BasicSprite.h"
#include <memory>

class DrawStaticImageHelper final: public IBasicHelper {
public:

    DrawStaticImageHelper(BasicSprite* image, const Position& pos, const size_t& clockwise_angle = 0);

    virtual void Paint(QPainter* painter) const override;
private:
    BasicSprite* image_;
    Position pos_;
    size_t angle_;
};