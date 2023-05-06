#pragma once

#include <qapplication.h>
#include "MainWidget.h"

class Render: public TMainWidget {
public:

    static Render& GetInstance();
    void RenderAll();

    ~Render();
private:
    Render();
    Render(const Render&) = delete;
    Render& operator=(const Render&) = delete;
};