#pragma once

#include "VisibleObject.h"

#include <qimage.h>
#include <QImage>
#include <string_view>

class BasicSprite: public IVisibleObject, public QImage{

public:
    BasicSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height, const RenderLayers& layer = RenderLayers::Objects);

    virtual void RenderIt(Painter *painter) override;
    void UpdateSrc(std::string_view image_src);
    bool IsDisplayed() const override;

    virtual ~BasicSprite() = default;

private:
    void Scale(const size_t& width, const size_t& height);
};
