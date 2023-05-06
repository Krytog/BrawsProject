#pragma once

#include "MainWidget.h"

class Render: public TMainWidget {
public:

    static Render& GetInstance();
    void RenderAll();

private:


private:
    Render();
    Render(const Render&) = delete;
    Render& operator=(const Render&) = delete;
};