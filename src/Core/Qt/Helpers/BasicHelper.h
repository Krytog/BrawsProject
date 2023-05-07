#pragma once

#include <concepts>
#include <qpainter.h>
#include <QPainter>
#include "../PaintTemplates/VisibleObject.h"
#include "../Painters/DummyPainter.h"

class IBasicHelper {
public:
    IBasicHelper() = default;

    virtual void Paint(Painter* painter) const = 0;

    virtual ~IBasicHelper() = default;
};