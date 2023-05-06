#pragma once

#include <concepts>
#include <qpainter.h>
#include <QPainter>
#include "../PaintTemplates/VisibleObject.h"

class IBasicHelper {
public:
    IBasicHelper() = default;

    virtual void Paint(QPainter* painter) const = 0;

    virtual ~IBasicHelper() = default;
};