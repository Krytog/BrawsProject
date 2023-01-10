#include "Canvas.h"

Canvas::Image::Image(const Position* pos, const double& width, const double& height): pos_(pos) {
        image_ = std::make_unique<sf::Image>();
        image_->create(width, height);
    }

void Canvas::Image::LoadFromFile(std::string_view path_to_file) {
    image_->loadFromFile(std::string(path_to_file));
}

void Canvas::Image::Clear() {
    image_.release();
}

Canvas::Image::~Image() = default;

Canvas::Canvas(const double& width, const double& height) {
    canvas_ = std::make_unique<sf::Image>();
    canvas_->create(width, height, sf::Color::Black);
}

void Canvas::Draw(const Canvas::Image* image) {
    const std::pair<double, double> image_pos = image->pos_->GetCoordinates();
    canvas_->copy(*(image->image_), image_pos.first, image_pos.second);
}

void Canvas::Clear() {
    canvas_.release();
}

Canvas::~Canvas() = default;