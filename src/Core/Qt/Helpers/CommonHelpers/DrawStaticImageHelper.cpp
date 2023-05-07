#include "DrawStaticImageHelper.h"
#include "../../Painters/QtPainter.h"

DrawStaticImageHelper::DrawStaticImageHelper(BasicSprite* image, const Position& pos,
                                             const size_t& clockwise_angle)
    : image_(image), pos_(pos), angle_(clockwise_angle){};

void DrawStaticImageHelper::Paint(Painter* painter) const {
    auto real_painter = dynamic_cast<QPainter*>(painter);
    real_painter->save();
    real_painter->rotate(angle_);
    const auto& coords = pos_.GetCoordinates();
    real_painter->drawImage(
        QRect{QPoint(coords.first - (image_->width() / 2), -coords.second - (image_->height() / 2)),
              QSize(image_->width(), image_->height())},
        *image_, QRect{QPoint(0, 0), QSize(image_->width(), image_->height())});
    real_painter->restore();
}