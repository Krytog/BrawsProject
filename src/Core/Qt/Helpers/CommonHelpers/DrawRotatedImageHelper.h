#pragma once

#include <qimage.h>
#include "../../../Position.h"
#include "DrawStaticImageHelper.h"
#include <memory>

class DrawRotatedImageHelper: public DrawStaticImageHelper {
public:
     DrawRotatedImageHelper(QImage* image, const Position& position, const Position& source_pos,
         const size_t& width, const size_t& height, size_t clockwise_angle);

    virtual void Paint(QPainter* painter) const override;

private:
    size_t clockwise_angle_;
};
