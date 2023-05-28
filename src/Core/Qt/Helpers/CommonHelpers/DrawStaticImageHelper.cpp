#include "DrawStaticImageHelper.h"
#include "../../Painters/QtPainter.h"
#include "../../MainWidget/Render.h"
#include <iostream>

DrawStaticImageHelper::DrawStaticImageHelper(QImage* image, const Position& pos,
                                             const double& clockwise_angle)
    : image_(image), pos_(pos), angle_(clockwise_angle){};

void DrawStaticImageHelper::Paint(Painter* painter) const {
    auto real_painter = dynamic_cast<QPainter*>(painter);
    real_painter->save();

    ////////
    if (!angle_) {
        auto bebra = Position(pos_.GetCoordinates().first, -pos_.GetCoordinates().second);
        auto camera_pos = Render::GetInstance().GetCameraPosition().GetCoordinates();
        bebra.Translate({-camera_pos.first, camera_pos.second});
        bebra.Translate(Vector2D(-(image_->width() / 2), -(image_->height() / 2)));
        auto left_corner_coords = bebra.GetCoordinates();
        QRect image(left_corner_coords.first, left_corner_coords.second, image_->width(), image_->height());
        QRect canvas(-(kWindowWidth / 2), -(kWindowHeight / 2), kWindowWidth, kWindowHeight);
        auto common = canvas.intersected(image);
        auto on_image = common.translated(-left_corner_coords.first, -left_corner_coords.second);
        common.translate(camera_pos.first, -camera_pos.second);
        QImage* real_image = new QImage(image_->copy(on_image));

        real_painter->drawImage(common, *real_image, real_image->rect());
    } else {
        const auto& coords = pos_.GetCoordinates();

        real_painter->translate(coords.first, -coords.second);
        real_painter->rotate(360 - angle_);
        real_painter->translate(- (image_->width() / 2), - (image_->height() / 2));

        real_painter->drawImage(QRect(0, 0, image_->width(), image_->height()),
             *image_, QRect(0, 0, image_->width(), image_->height()));
    }

    real_painter->restore();
}