#pragma once

#include <qimage.h>
#include "../../../Position.h"
#include "../BasicHelper.h"
#include <memory>

class DrawStaticImageHelper : public IBasicHelper {
public:
    DrawStaticImageHelper(QImage* image, const Position& position, const Position& source_pos,
         const size_t& width, const size_t& height);

    virtual void Paint(QPainter* painter) const override;

protected:
    void DrawImage(QPainter* painter) const;

private:
    QImage* image_;
    Position pos_;
    Position source_pos_;
    size_t width_;
    size_t height_;
};