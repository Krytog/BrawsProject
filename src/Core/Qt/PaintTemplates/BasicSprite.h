#pragma once

#include "VisibleObject.h"

#include <qimage.h>
#include <QImage>
#include <string_view>

class BasicSprite: public IVisibleObject, public QImage{

public:
    BasicSprite(std::string_view image_src, const Position& position,
         const size_t& width, const size_t& height);

    virtual void RenderIt(QPainter *painter) override;
    void UpdateSrc(std::string_view image_src);

    virtual ~BasicSprite() = default;
};
