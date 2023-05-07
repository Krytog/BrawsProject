#pragma once

#include "DummyPainter.h"
#include "QtPainter.h"
#include <QPainter>

class QtPainter: public Painter, public QPainter {
public:
    QtPainter() = default;
};
