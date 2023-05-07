#pragma once

#include "VisibleObject.h"
#include "../Helpers/BasicHelper.h"
#include "../../EventHandler.h"

#include <qimage.h>
#include <QImage>
#include <string_view>

class AnimatedSprite: public IFlexibleVisibleObject, public QImage {
    friend class DrawAnimatedImageHelper;
public:
    AnimatedSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height, const size_t& ticks_per_frame, const size_t& frames_count,
            bool is_cycled = true, const RenderLayers& layer = RenderLayers::Objects);

    virtual void RenderItCustom(Painter *painter) override;
    void UpdateSrc(std::string_view image_src, const size_t& ticks_per_frame, const size_t& frames_count);
    bool IsDisplayed() const override;

    virtual ~AnimatedSprite();

private:
    size_t frames_count_;
    size_t ticks_per_frame_;
    bool is_cycled_;

    size_t cur_frame_ = 0;
    Event* animation_event_ = nullptr;

private:
    void ResetAnimationEvent();

    void Scale(const size_t& width, const size_t& height);
};