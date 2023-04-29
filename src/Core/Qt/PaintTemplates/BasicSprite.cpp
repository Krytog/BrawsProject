#include "BasicSprite.h"
#include <qimage.h>
#include <memory>
#include "VisibleObject.h"
#include "../Helpers/CommonHelpers/DrawStaticImageHelper.h"

BasicSprite::BasicSprite(std::string_view image_src, const Position& position,
        const size_t& width, const size_t& height):
            IVisibleObject(position), QImage(image_src.data()) 
{
    if (width != this->width() || height != this->height()) {
        *dynamic_cast<QImage*>(this) = std::move(this->scaled(width, height, Qt::KeepAspectRatio));
    }
};

void BasicSprite::RenderIt(QPainter *painter) {
    DrawStaticImageHelper(std::shared_ptr<QImage>(this),  pos_).Paint(painter);
}