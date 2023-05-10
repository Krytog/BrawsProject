#include "DrawAnimatedImageHelper.h"
#include <iostream>
#include <ostream>
#include "../../Painters/QtPainter.h"

DrawAnimatedImageHelper::DrawAnimatedImageHelper(AnimatedSprite* image, const Position& pos,
                                                 const size_t& clockwise_angle)
    : image_(image), pos_(pos), angle_(clockwise_angle) {
}

void DrawAnimatedImageHelper::Paint(Painter* painter) const {
    auto real_painter = dynamic_cast<QPainter*>(painter);
    real_painter->save();

    auto coords = pos_.GetCoordinates();
    int64_t frame_width = image_->width() / image_->frames_count_;

    /* Вынести потом куда-то */
    real_painter->translate(coords.first, -coords.second);
    real_painter->rotate(angle_);
    real_painter->translate(- (frame_width / 2), - (image_->height() / 2));

    real_painter->drawImage(
        QRect{QPoint(0, 0),
              QSize(frame_width, image_->height())},
        *image_,
        QRect{QPoint(frame_width * image_->cur_frame_, 0), QSize(frame_width, image_->height())});
    real_painter->restore();
}
