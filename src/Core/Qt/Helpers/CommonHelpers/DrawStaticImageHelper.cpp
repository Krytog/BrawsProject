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


    //////// Temporary fix /////////
    if (!angle_) {
        auto result_position = Position(pos_.GetCoordinates().first, -pos_.GetCoordinates().second);
        auto camera_position = Render::GetInstance().GetCameraPosition().GetCoordinates();

        result_position.Translate({-camera_position.first, camera_position.second});
        result_position.Translate(Vector2D(-(image_->width() / 2), -(image_->height() / 2)));
    
        auto left_corner_coords = result_position.GetCoordinates();
        QRect image(left_corner_coords.first, left_corner_coords.second, image_->width(), image_->height());
        QRect canvas(-(kWindowWidth / 2), -(kWindowHeight / 2), kWindowWidth, kWindowHeight);
        auto intersection_rect = canvas.intersected(image);

        auto original_image_rect = intersection_rect.translated(-left_corner_coords.first, -left_corner_coords.second);
        intersection_rect.translate(camera_position.first, -camera_position.second);

        QImage* visible;
        if (dynamic_cast<BasicSprite*>(image_)) {
            visible = &dynamic_cast<BasicSprite*>(image_)->GetVisiblePart();
            *visible = image_->copy(original_image_rect);
        } else if (dynamic_cast<BasicFlexibleSprite*>(image_)) {
            visible = &dynamic_cast<BasicFlexibleSprite*>(image_)->GetVisiblePart();
            *visible = image_->copy(original_image_rect);
        }

        real_painter->drawImage(intersection_rect, *visible, visible->rect());
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