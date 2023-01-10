#include <memory>

#include "Sprites.h"
#include "Canvas.h"

class VisibleObjectImpl {
public:
    VisibleObjectImpl(const Position* pos, const double& width, const double& height): top_left_(*pos),
                        width_(width), height_(height) {};

    virtual void UpdatePosition(const Position &position) {
        top_left_ = position;
    };

    virtual void Translate(const Vector2D &vector2D) {
        top_left_.Translate(vector2D);
    };

    virtual void RenderIt(Canvas* canvas) = 0;
protected:
    Position top_left_;
    double width_;
    double height_;
};

class StaticSpriteImpl: public VisibleObjectImpl {
public:

    StaticSpriteImpl(const Position *pos, const double &width, const double &height,
                     std::string_view path_to_file): VisibleObjectImpl(pos, width, height), image_(pos, width, height) {
        image_.LoadFromFile(path_to_file);
    }

    void RenderIt(Canvas *canvas) override {
        canvas->Draw(&image_);
    }

private:
    Canvas::Image image_;
};

StaticSprite::StaticSprite(const Position *pos, const double &width, const double &height,
                           std::string_view path_to_file) {
    impl_ = std::make_unique<StaticSpriteImpl>(pos, width, height, path_to_file);
}

void StaticSprite::UpdatePosition(const Position &position) {
    impl_->UpdatePosition(position);
}

void StaticSprite::Translate(const Vector2D &vector2D) {
    impl_->Translate(vector2D);
}

void StaticSprite::RenderIt(Canvas *canvas) const {
    impl_->RenderIt(canvas);
}

StaticSprite::~StaticSprite() noexcept = default;

class AnimatedSpriteImpl: public VisibleObjectImpl {
public:
    AnimatedSpriteImpl(const Position *pos, const double &width, const double &height,
                     std::string_view path_to_file, size_t frame_rate): VisibleObjectImpl(pos, width, height),
                     image_(pos, width, height), frame_rate_(frame_rate) {
        image_.LoadFromFile(std::string(path_to_file));
    };

    void RenderIt(Canvas *canvas) override {
        canvas->Draw(&image_);
    }
    
private:
    Canvas::Image image_;
    size_t frame_rate_;
};

AnimatedSprite::AnimatedSprite(const Position *pos, const double &width, const double &height,
                               std::string_view path_to_file, size_t frame_rate) {
    impl_ = std::make_unique<AnimatedSpriteImpl>(pos, width, height, path_to_file, frame_rate);
}

void AnimatedSprite::RenderIt(Canvas *canvas) const {
    impl_->RenderIt(canvas);
}

void AnimatedSprite::UpdatePosition(const Position &position) {
    impl_->UpdatePosition(position);
}

void AnimatedSprite::Translate(const Vector2D &vector2D) {
    impl_->Translate(vector2D);
}

AnimatedSprite::~AnimatedSprite() noexcept = default;