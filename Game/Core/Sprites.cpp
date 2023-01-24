#include <memory>
#include <list>
// Log
#include <iostream>

#include "Sprites.h"
#include "Canvas.h"

// Basic Implementation class
class VisibleObjectImpl {
public:
    VisibleObjectImpl(const Position *pos, const size_t &width, const size_t &height,
                      const size_t &render_level, bool is_cycled = true)
        : pos_(*pos),
          width_(width),
          height_(height),
          render_level_(render_level),
          is_cycled_(is_cycled){};

    virtual void UpdatePosition(const Position &position) {
        pos_ = position;
    };

    virtual void Translate(const Vector2D &vector2D) {
        pos_.Translate(vector2D);
    };

    size_t GelRenderLevel() const {
        return render_level_;
    }

    virtual void RenderIt(Canvas *canvas) = 0;

protected:
    Position pos_;  // Coincides with the center of the object, that is, the usual local Position of
                    // the GameObject, where Render will draw it
    size_t width_;
    size_t height_;

    size_t render_level_;

    bool is_finished_ = false;  // Object animation complete
    bool is_cycled_ = true;     // Object animation is cycled, only meaningful for animated sprites
};

class StaticSpriteImpl : public VisibleObjectImpl {
public:
    StaticSpriteImpl(const Position *pos, const size_t &width, const size_t &height,
                     std::string_view path_to_file, const size_t &render_level)
        : VisibleObjectImpl(pos, width, height, render_level), image_(&pos_, width, height) {
        image_.LoadFromFile(path_to_file);
    }

    void RenderIt(Canvas *canvas) override {
        canvas->Draw(&image_);
        if (!is_finished_) {
            is_finished_ = true;
        }
    }

private:
    Canvas::Image image_;
};

StaticSprite::StaticSprite(const Position *pos, const size_t &width, const size_t &height,
                           std::string_view path_to_file, const size_t &render_level) {
    impl_ = std::make_unique<StaticSpriteImpl>(pos, width, height, path_to_file, render_level);
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

size_t StaticSprite::GetRenderLevel() const {
    return impl_->GelRenderLevel();
}

StaticSprite::~StaticSprite() noexcept = default;

class AnimatedSpriteImpl : public VisibleObjectImpl {
public:
    AnimatedSpriteImpl(const Position *pos, const size_t &width, const size_t &height,
                       std::string_view path_to_file, const size_t &render_level,
                       const size_t &frame_rate, const size_t &frames_count_width,
                       const size_t &frames_count_height, bool is_cycled)
        : VisibleObjectImpl(pos, width, height, render_level, is_cycled),
          image_(&pos_, width, height),
          frame_rate_(frame_rate),
          frames_count_width_(frames_count_width),
          frames_count_height_(frames_count_height) {
        static_image_.LoadFromFile(std::string(path_to_file));
        FRAME_SIZE_X = static_cast<double>(static_image_.GetRealSize().first) / frames_count_width_;
        FRAME_SIZE_Y =
            static_cast<double>(static_image_.GetRealSize().second) / frames_count_height;

        // Log
        std::cout << static_image_.GetRealSize().first << " " << static_image_.GetRealSize().second
                  << "\n";
        std::cout << frames_count_width << " " << frames_count_height << "\n";
    };

    void RenderIt(Canvas *canvas) override {
        if (is_finished_ && !is_cycled_) {
            canvas->Draw(&image_);
            return;
        }

        if (++current_rate_ == frame_rate_) {  // Frame ended
            current_rate_ = 0;
            if (++current_frame_x == frames_count_width_) {  // The last frame in the line is played
                current_frame_x = 0;
                if (++current_frame_y ==
                    frames_count_height_) {  // The last frame in the sprite has been played
                    current_frame_y = 0;
                    if (!is_finished_) {
                        is_finished_ = true;
                    }
                    if (!is_cycled_) {
                        canvas->Draw(&image_);
                        return;
                    }
                }
            }
        }

        image_.LoadFromStaticImage(
            static_image_, Position(0, 0),
            Position(current_frame_x * FRAME_SIZE_X, current_frame_y * FRAME_SIZE_Y),
            Position((current_frame_x + 1) * FRAME_SIZE_X, (current_frame_y + 1) * FRAME_SIZE_Y),
            FRAME_SIZE_X, FRAME_SIZE_Y);
        canvas->Draw(&image_);
    }

    bool AnimationIsFinished() const {
        return is_finished_;
    };

private:
    Canvas::StaticImage static_image_;  // All frames
    Canvas::Image image_;               // Current frame image

    const size_t frames_count_width_;
    const size_t frames_count_height_;

    const size_t frame_rate_;

    size_t current_frame_x = 0;
    size_t current_frame_y = 0;

    size_t current_rate_ = 0;

    // Frame size on real image
    size_t FRAME_SIZE_X;
    size_t FRAME_SIZE_Y;
};

AnimatedSprite::AnimatedSprite(const Position *pos, const size_t &width, const size_t &height,
                               std::string_view path_to_file, const size_t &render_level,
                               const size_t &frame_rate, const size_t &frames_count_width,
                               const size_t &frames_count_height, bool is_cycled) {
    impl_ = std::make_unique<AnimatedSpriteImpl>(pos, width, height, path_to_file, render_level,
                                                 frame_rate, frames_count_width,
                                                 frames_count_height, is_cycled);
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

size_t AnimatedSprite::GetRenderLevel() const {
    return impl_->GelRenderLevel();
}

AnimatedSprite::~AnimatedSprite() noexcept = default;

//
template <class T>
class BasicSequncer {

    struct DisplayInfo {
        T* object;
        std::string_view tag;
    };

public:
//    BasicSequncer(const std::initializer_list<DisplayInfo>& objects_list) {
//        for (c)
//    }

private:
    std::list<DisplayInfo> display_objects_;
};

class VisibleSequncer: public BasicSequncer<VisibleObject>, VisibleObject {
public:

private:

};