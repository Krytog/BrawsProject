#include "BasicSprite.h"
#include <qimage.h>
#include <memory>
#include "VisibleObject.h"
#include "../Helpers/CommonHelpers/DrawStaticImageHelper.h"
#include <iostream>

BasicSprite::BasicSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height, const double& angle, const RenderLayers& layer):
            IVisibleObject(position, angle, layer), QImage(image_src.data()) 
{
    Scale(width, height);
};

void BasicSprite::RenderIt(Painter *painter) {
    DrawStaticImageHelper(this, pos_, angle_).Paint(painter);
}

bool BasicSprite::IsDisplayed() const {
    return true;
}

void BasicSprite::Scale(const size_t& width, const size_t& height) {
    if (width != this->width() || height != this->height()) {
        *dynamic_cast<QImage*>(this) = std::move(this->scaled(width, height, Qt::KeepAspectRatio));
    }
}

BasicFlexibleSprite::BasicFlexibleSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height, const double& angle, const RenderLayers& layer):
            IFlexibleVisibleObject(position, angle, layer), QImage(image_src.data()) 
{
    Scale(width, height);
};

void BasicFlexibleSprite::Scale(const size_t& width, const size_t& height) {
    if (width != this->width() || height != this->height()) {
        *dynamic_cast<QImage*>(this) = std::move(this->scaled(width, height, Qt::KeepAspectRatio));
    }
}

void BasicFlexibleSprite::UpdateSrc(std::string_view image_src) {
    const size_t& cur_width = this->width();
    const size_t& cur_height = this->height();
    load(image_src.data());
    Scale(cur_width, cur_height);
}

void BasicFlexibleSprite::RenderItCustom(Painter* painter) {
    DrawStaticImageHelper(this, pos_, angle_).Paint(painter);
}
