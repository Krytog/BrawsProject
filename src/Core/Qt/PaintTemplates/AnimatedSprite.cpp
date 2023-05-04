#include "AnimatedSprite.h"
#include <algorithm>
#include <cstddef>
#include "VisibleObject.h"
#include "../Helpers/CommonHelpers/DrawStaticImageHelper.h"
#include "../Helpers/CommonHelpers/DrawRotatedImageHelper.h"

AnimatedSprite::AnimatedSprite(std::string_view image_src, const Position& position, const size_t& width,
                               const size_t& height, const size_t& ticks_per_frame,
                               const size_t& frames_count, bool is_cycled, const RenderLayers& layer)
    : IFlexibleVisibleObject(position, layer), QImage(image_src.data()), 
      ticks_per_frame_(ticks_per_frame),
      frames_count_(frames_count),
      is_cycled_(is_cycled) {
    Scale(frames_count * width, height);
}

void AnimatedSprite::RenderItCustom(QPainter* painter) {
    Update(); /* Refactor using Events later */
    DrawStaticImageHelper(this, pos_, Position(cur_frame_ * (this->width() / frames_count_), 0), this->width() / frames_count_, this->height())
        .Paint(painter);
}

void AnimatedSprite::UpdateSrc(std::string_view image_src, const size_t& ticks_per_frame,
                               const size_t& frames_count) {
    const size_t& width = this->width() / frames_count_;
    const size_t& height = this->height();

    this->load(image_src.data());
    ticks_per_frame_ = ticks_per_frame;
    frames_count_ = frames_count;

    Scale(width * frames_count_, height);

    { /* Reafactor using Events */
        cur_frame_ = 0;
        cur_tick_ = 0;
    }
}

bool AnimatedSprite::IsDisplayed() const {
    return !is_cycled_ && (cur_frame_ == frames_count_ - 1);
}

void AnimatedSprite::Update() {
    if (cur_tick_ == ticks_per_frame_) {
        cur_frame_ =
            (is_cycled_) ? ((cur_frame_ + 1) % frames_count_) : (std::min(frames_count_ - 1, cur_frame_ + 1));
        cur_tick_ = 0;
    }
    ++cur_tick_;
}

void AnimatedSprite::Scale(const size_t& width, const size_t& height) {
    if (width != this->width() || height != this->height()) {
        *dynamic_cast<QImage*>(this) = std::move(this->scaled(width, height, Qt::KeepAspectRatio));
    }
}
