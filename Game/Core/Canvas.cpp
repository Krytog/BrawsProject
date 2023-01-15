#include "Canvas.h"

Canvas::Image::Image(const Position *pos, const double &width, const double &height)
    : pos_(pos), width_(width), height_(height) {
    image_ = std::make_unique<sf::Image>();
}

void Canvas::Image::LoadFromFile(std::string_view path_to_file) {
    image_->loadFromFile(std::string(path_to_file));
    Resize(width_, height_);
}

// Тут пока написана хуйня, но это потом
void Canvas::Image::Clear() {
    image_.reset();
}

void Canvas::Image::Resize(int width, int height) {
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
    const std::pair<double, double> image_pos = image->pos_->GetCoordinates();
    //  canvas_->copy(*(image->image_), image_pos.first, image_pos.second);
    canvas_->copy(*(image->image_), kFrameWidth + image_pos.first,
                  kWindowHeight + kFrameHeight - image->height_ - image_pos.second);
}

// Тут пока написана хуйня, но это потом
void Canvas::Clear() {
    canvas_.reset();
}

Canvas::~Canvas() = default;
