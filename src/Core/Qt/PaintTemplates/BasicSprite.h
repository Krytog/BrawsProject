#pragma once

#include "VisibleObject.h"

#include <qimage.h>
#include <QImage>
#include <cstddef>
#include <string_view>

class BasicSprite final: public IVisibleObject, public QImage{

public:
    BasicSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height, const double& angle = 0,
             const RenderLayers& layer = RenderLayers::Objects);

    virtual void RenderIt(Painter *painter) override;
    bool IsDisplayed() const override;
    QImage& GetVisiblePart();

    virtual ~BasicSprite() = default;

    void Scale(const size_t& width, const size_t& height) override; /* TODO: вынести это в VisibleObject.h */
private:
    QImage visible_part_;
};

class BasicFlexibleSprite: public IFlexibleVisibleObject, public QImage {
public:
    BasicFlexibleSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height, const double& angle = 0,
             const RenderLayers& layer = RenderLayers::Objects);

    virtual void RenderItCustom(Painter* painter) override;
    void UpdateSrc(std::string_view image_src);
    QImage& GetVisiblePart();

    virtual ~BasicFlexibleSprite() = default;

    void Scale(const size_t& width, const size_t& height) override; /* TODO: вынести это в VisibleObject.h */
private:
    QImage visible_part_;
};