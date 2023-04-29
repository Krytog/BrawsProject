#include "DrawStaticImageHelper.h"
#include <qpoint.h>
#include <qsize.h>

void DrawStaticImageHelper::Configure(std::shared_ptr<QImage> image, const Position& position) {
    image_ = std::move(image);
    pos_ = position;
}

DrawStaticImageHelper::DrawStaticImageHelper(std::shared_ptr<QImage> image, const Position& position) {
    Configure(std::move(image), position);
}

void DrawStaticImageHelper::Paint(QPainter* painter) const {
    painter->save();
    const auto& coords = pos_.GetCoordinates();
    painter->drawImage(
        QRect{QPoint(coords.first - (image_->width() / 2), -coords.second - (image_->height() / 2)),
              QSize(image_->width(), image_->height())},
        *image_, QRect{QPoint{0, 0}, QSize{image_->width(), image_->height()}});
    painter->restore();
}