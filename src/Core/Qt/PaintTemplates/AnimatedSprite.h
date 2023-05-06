#pragma once

#include "VisibleObject.h"
#include "../Helpers/BasicHelper.h"

#include <qimage.h>
#include <QImage>
#include <string_view>

class AnimatedSprite: public IFlexibleVisibleObject, public QImage {
    friend class DrawAnimatedImageHelper;
public:
    AnimatedSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height, const size_t& ticks_per_frame, const size_t& frames_count,
            bool is_cycled = true, const RenderLayers& layer = RenderLayers::CharactersLayer);

    virtual void RenderItCustom(QPainter *painter) override;
    void UpdateSrc(std::string_view image_src, const size_t& ticks_per_frame, const size_t& frames_count);
    bool IsDisplayed() const override;

    virtual ~AnimatedSprite() = default;

private:
    size_t frames_count_;
    size_t cur_frame_ = 0;
    bool is_cycled_;

    /* Refactor using Events later */
    size_t ticks_per_frame_;
    size_t cur_tick_ = 0;

private:
    void Update();

    void Scale(const size_t& width, const size_t& height);
};