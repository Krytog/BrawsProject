#pragma once

#include <qimage.h>
#include "../../../Position.h"
#include "../BasicHelper.h"
#include"../../PaintTemplates/BasicSprite.h"
#include <memory>

class DrawStaticImageHelper final: public IBasicHelper {
public:

    DrawStaticImageHelper(BasicSprite* image, const Position& pos, const double& clockwise_angle = 0);

    virtual void Paint(Painter* painter) const override;
private:
    BasicSprite* image_;
    Position pos_;
    double angle_;
};