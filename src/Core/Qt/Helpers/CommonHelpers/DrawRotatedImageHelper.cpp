#include "DrawRotatedImageHelper.h"
#include "DrawStaticImageHelper.h"

DrawRotatedImageHelper::DrawRotatedImageHelper(QImage* image, const Position& position, const Position& source_pos,
         const size_t& width, const size_t& height, size_t clockwise_angle): DrawStaticImageHelper(image, position, source_pos,
            width, height), clockwise_angle_(clockwise_angle) {}

void DrawRotatedImageHelper::Paint(QPainter *painter) const {
    painter->save();
    painter->rotate(clockwise_angle_);
    DrawImage(painter);
    painter->restore();
}