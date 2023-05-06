#pragma once

#include <qapplication.h>
#include "MainWidget.h"

class Render: public TMainWidget {
public:

    static Render& GetInstance();
    void RenderAll();

    ~Render();
private:
    int a_ = 1;

private:
    Render();
    Render(const Render&) = delete;
    Render& operator=(const Render&) = delete;
};