#include "DrawStaticImageHelper.h"
#include "../../Painters/QtPainter.h"

DrawStaticImageHelper::DrawStaticImageHelper(BasicSprite* image, const Position& pos,
                                             const double& clockwise_angle)
    : image_(image), pos_(pos), angle_(clockwise_angle){};

void DrawStaticImageHelper::Paint(Painter* painter) const {
    auto real_painter = dynamic_cast<QPainter*>(painter);
    real_painter->save();

    const auto& coords = pos_.GetCoordinates();

    real_painter->translate(coords.first, -coords.second);
    real_painter->rotate(360 - angle_);
    real_painter->translate(- (image_->width() / 2), - (image_->height() / 2));

    real_painter->drawImage(
        QRect{QPoint(0, 0), QSize(image_->width(), image_->height())},
        *image_, QRect{QPoint(0, 0), QSize(image_->width(), image_->height())});
    
    real_painter->restore();
}