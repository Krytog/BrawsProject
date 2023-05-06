#include "BasicSprite.h"
#include <qimage.h>
#include <memory>
#include "VisibleObject.h"
#include "../Helpers/CommonHelpers/DrawStaticImageHelper.h"
#include <iostream>

BasicSprite::BasicSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height, const RenderLayers& layer):
            IVisibleObject(position, layer), QImage(image_src.data()) 
{
    Scale(width, height);
};

void BasicSprite::RenderIt(QPainter *painter) {
    DrawStaticImageHelper(this, pos_).Paint(painter);
}

void BasicSprite::UpdateSrc(std::string_view image_src) {
    const size_t& cur_width = this->width();
    const size_t& cur_height = this->height();
    load(image_src.data());
    Scale(cur_width, cur_height);
}

bool BasicSprite::IsDisplayed() const {
    return true;
}

void BasicSprite::Scale(const size_t& width, const size_t& height) {
    if (width != this->width() || height != this->height()) {
        *dynamic_cast<QImage*>(this) = std::move(this->scaled(width, height, Qt::KeepAspectRatio));
    }
}