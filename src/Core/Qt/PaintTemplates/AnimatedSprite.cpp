#include "AnimatedSprite.h"
#include "../Helpers/CommonHelpers/DrawAnimatedImageHelper.h"
#include "../RecourcesInitializer/RecourcesInitializer.h"
#include <algorithm>
#include <cstddef>
#include "VisibleObject.h"
#include <iostream>

AnimatedSprite::AnimatedSprite(std::string_view image_src, const Position& position, const size_t& width,
                               const size_t& height, const size_t& ticks_per_frame,
                               const size_t& frames_count, bool is_cycled, const double& angle, const RenderLayers& layer)
    : IFlexibleVisibleObject(position, angle, layer), QImage(RecourcesInitializer::GetInstance().GetImage(image_src.data())), 
      ticks_per_frame_(ticks_per_frame),
      frames_count_(frames_count),
      is_cycled_(is_cycled) {
    Scale(frames_count * width, height);
    ResetAnimationEvent();
}

void AnimatedSprite::RenderItCustom(Painter* painter) {
    DrawAnimatedImageHelper(this, pos_, angle_).Paint(painter);
}

void AnimatedSprite::UpdateSrc(std::string_view image_src, const size_t& ticks_per_frame,
                               const size_t& frames_count) {
    const size_t& width = this->width() / frames_count_;
    const size_t& height = this->height();

    load(image_src.data());
    ticks_per_frame_ = ticks_per_frame;
    frames_count_ = frames_count;

    Scale(width * frames_count_, height);
    ResetAnimationEvent();
}

bool AnimatedSprite::IsDisplayed() const {
    return !is_cycled_ && (cur_frame_ == frames_count_ - 1);
}

void AnimatedSprite::Scale(const size_t& width, const size_t& height) {
    if (width != this->width() || height != this->height()) {
        *dynamic_cast<QImage*>(this) = std::move(this->scaled(width, height, Qt::IgnoreAspectRatio));
    }
}

void AnimatedSprite::ResetAnimationEvent() {
    cur_frame_ = 0;
    if (animation_event_) {
        EventHandler::GetInstance().DestroyEvent(animation_event_);
    }

    auto condition = [this]() { 
        static int cur_tick = 0;
        if (cur_tick++ == ticks_per_frame_) {
            cur_tick = 0;
            return true;
        } 
        return false;
    };

    auto update = [this]() {
        cur_frame_ = (is_cycled_) ? ((cur_frame_ + 1) % frames_count_) :
            (std::min(frames_count_ - 1, cur_frame_ + 1));
    };

    animation_event_ = EventHandler::GetInstance().CreateEvent(condition,
     {}, update, {}, EventStatus::Reusable);
}

AnimatedSprite::~AnimatedSprite() {
    if (animation_event_) {
        EventHandler::GetInstance().DestroyEvent(animation_event_);
    }
}