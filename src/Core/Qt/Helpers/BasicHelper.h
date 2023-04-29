#pragma once

#include <qpainter.h>
#include <QPainter>

class IBasicHelper {
public:
    IBasicHelper() = default;

    virtual void Paint(QPainter* painter) const = 0;

    virtual ~IBasicHelper() = default;
};