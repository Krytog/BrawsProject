#pragma once

#include <qimage.h>
#include "../../../Position.h"
#include "../BasicHelper.h"
#include <memory>

class DrawStaticImageHelper : public IBasicHelper {
public:
    DrawStaticImageHelper() = default;
    DrawStaticImageHelper(std::shared_ptr<QImage> image, const Position& position);
    void Configure(std::shared_ptr<QImage> image, const Position& position);

    virtual void Paint(QPainter* painter) const override;

private:
    std::shared_ptr<QImage> image_;
    Position pos_;
};