#include "DrawAnimatedImageHelper.h"
#include "../../Painters/QtPainter.h"

DrawAnimatedImageHelper::DrawAnimatedImageHelper(AnimatedSprite* image, const Position& pos,
                                                 const size_t& clockwise_angle)
    : image_(image), pos_(pos), angle_(clockwise_angle) {
}

void DrawAnimatedImageHelper::Paint(Painter* painter) const {
    auto real_painter = dynamic_cast<QPainter*>(painter);
    real_painter->save();
    real_painter->rotate(angle_);
    auto coords = pos_.GetCoordinates();
    size_t frame_width = image_->width() / image_->frames_count_;
    real_painter->drawImage(
        QRect{QPoint(coords.first - (frame_width / 2), -coords.second - (image_->height() / 2)),
              QSize(frame_width, image_->height())},
        *image_,
        QRect{QPoint(frame_width * image_->cur_frame_, 0), QSize(frame_width, image_->height())});
    real_painter->restore();
}
