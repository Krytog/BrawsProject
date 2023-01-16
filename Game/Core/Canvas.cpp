#include "Canvas.h"

#include <iostream>

#define LOCAL_CENTER_X (kCanvasWidth / 2)
#define LOCAL_CENTER_Y (kCanvasHeight / 2)

Canvas::Image::Image(const Position *pos, const double &width, const double &height)
    : pos_(pos), width_(width), height_(height) {
    image_ = std::make_unique<sf::Image>();
}

void Canvas::Image::LoadFromFile(std::string_view path_to_file) {
    image_->loadFromFile(std::string(path_to_file));
    Resize(width_, height_);
}

void Canvas::Image::Clear() {
    image_.reset();
}

void Canvas::Image::Resize(uint32_t width, uint32_t height) {
    sf::Image *tmp_resize_image = new sf::Image();
    (*tmp_resize_image).create(width, height);
    ResizeImage(*image_, *tmp_resize_image);
    image_.reset(tmp_resize_image);
}

void Canvas::Image::ResizeImage(const sf::Image &originalImage, sf::Image &resizedImage) {
    const sf::Vector2u originalImageSize{originalImage.getSize()};
    const sf::Vector2u resizedImageSize{resizedImage.getSize()};
    for (unsigned int y{0u}; y < resizedImageSize.y; ++y) {
        for (unsigned int x{0u}; x < resizedImageSize.x; ++x) {
            unsigned int origX{static_cast<unsigned int>(static_cast<double>(x) /
                                                         resizedImageSize.x * originalImageSize.x)};
            unsigned int origY{static_cast<unsigned int>(static_cast<double>(y) /
                                                         resizedImageSize.y * originalImageSize.y)};
            resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
        }
    }
}

Canvas::Image::~Image() = default;

Canvas::Canvas(const double &width, const double &height) {
    canvas_ = std::make_unique<sf::Image>();
    canvas_->create(width, height, sf::Color::Black);
}

void Canvas::Draw(const Canvas::Image *image) {
    const Position *global_pos = image->pos_;
    const Vector2D offset =
        global_pos->GetCoordinatesAsVector2D() - global_center_.GetCoordinatesAsVector2D();
    const double local_pos_x = LOCAL_CENTER_X + offset.GetCoordinates().first - (image->width_ / 2);
    const double local_pos_y =
        LOCAL_CENTER_Y - offset.GetCoordinates().second - (image->height_ / 2);
    if (OnCanvas(local_pos_x, local_pos_y)) {
        canvas_->copy(*image->image_, local_pos_x, local_pos_y);
    }
}

void Canvas::Clear() {
    canvas_.reset();
}

void Canvas::SetCenter(Position global_center) {
    global_center_ = global_center;
}

bool Canvas::OnCanvas(double x, double y) {
    return (x > 0 && x < kCanvasWidth) && (y > 0 && y < kCanvasWidth);
}

Canvas::~Canvas() = default;
