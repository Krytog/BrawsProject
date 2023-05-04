#include "DrawStaticImageHelper.h"
#include <qpoint.h>
#include <qsize.h>

#include <iostream>

DrawStaticImageHelper::DrawStaticImageHelper(
    QImage* image, const Position& position,
    const Position& source_pos,
    const size_t& width, const size_t& height
): image_(image), pos_(position), source_pos_(source_pos), width_(width), height_(height) {}

void DrawStaticImageHelper::Paint(QPainter* painter) const {
    painter->save();
    DrawImage(painter);
    painter->restore();
}

void DrawStaticImageHelper::DrawImage(QPainter* painter) const {
    const auto& coords = pos_.GetCoordinates();
    const auto& source_coords = source_pos_.GetCoordinates();
    painter->drawImage(
        QRect{QPoint(coords.first - (width_ / 2), -coords.second - (height_ / 2)),
              QSize(width_, height_)},
        *image_, QRect{QPoint(source_coords.first, source_coords.second), QSize(width_, height_)});
}
