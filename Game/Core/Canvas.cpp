#include "Canvas.h"

#include <iostream>

#define LOCAL_CENTER_X (kWindowWidth / 2)
#define LOCAL_CENTER_Y (kWindowHeight / 2)

Canvas::Image::Image(const Position &pos, const size_t &width, const size_t &height)
    : pos_(pos), width_(width), height_(height){};

void Canvas::Image::LoadFromFile(std::string_view path_to_file) {
    image_.loadFromFile(path_to_file.data());
}

void Canvas::StaticImage::LoadFromFile(std::string_view path_to_file) {
    image_.loadFromFile(path_to_file.data());
}

void Canvas::StaticImage::SetDefaultImage() {
    image_.create(100, 100); /* Need to fix Later */
}

std::pair<size_t, size_t> Canvas::StaticImage::GetRealSize() const {
    const auto &vector_size = image_.getSize();
    return {vector_size.x, vector_size.y};
}
const sf::Image &Canvas::StaticImage::GetEntireImage() const {
    return image_;
}

void Canvas::Image::LoadFromStaticImage(const StaticImage &static_image,
                                        const Position &orig_image_pos,
                                        const Position &rectangle_left_up_corner,
                                        const Position &rectangle_right_down_corner,
                                        const size_t &orig_width, const size_t &orig_height) {
    sf::Image frame_image;
    frame_image.create(orig_width, orig_height);

    const auto &left_up_coords = rectangle_left_up_corner.GetCoordinates();
    const auto &right_down_coords = rectangle_right_down_corner.GetCoordinates();

    frame_image.copy(static_image.GetEntireImage(), 0, 0,
                     sf::IntRect(left_up_coords.first, left_up_coords.second,
                                 right_down_coords.first, right_down_coords.second));
    image_.loadFromImage(frame_image);
}

void Canvas::Image::UpdatePosition(const Position& position) {
    pos_ = position;
}

void Canvas::Image::Translate(const Vector2D& vector2D) {
    pos_.Translate(vector2D);
}

Canvas::Image::~Image() = default;

Canvas::Canvas(sf::RenderWindow *window_ptr_) : canvas_(window_ptr_){};

void Canvas::Draw(Canvas::Image *image) {
    const Position global_pos = image->pos_;
    const Vector2D offset =
        global_pos.GetCoordinatesAsVector2D() - global_center_.GetCoordinatesAsVector2D();
    const long long local_pos_x =
        LOCAL_CENTER_X + offset.GetCoordinates().first - (image->width_ / 2);
    const long long local_pos_y =
        LOCAL_CENTER_Y - offset.GetCoordinates().second - (image->height_ / 2);

    const double scale_factor_x = static_cast<double>(image->width_) / image->image_.getSize().x;
    const double scale_factor_y = static_cast<double>(image->height_) / image->image_.getSize().y;
    sf::Sprite sprite(image->image_);
    sprite.setScale(scale_factor_x, scale_factor_y);
    sprite.setPosition(local_pos_x, local_pos_y);
    canvas_->draw(sprite);
}

void Canvas::SetCenter(Position global_center) {
    global_center_ = global_center;
}

void Canvas::Image::SetDefaultImage() {
    image_.create(width_, height_);
}

Canvas::~Canvas() = default;
