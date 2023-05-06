#include "Render.h"
#include <qapplication.h>
#include "MainWidget.h"
#include <QApplication>
#include <cstdlib>

//////////////////// QApp initialization /////////////////////////////
static int k_dummy_argc = 1; /* Fuck qt */
static QApplication app = QApplication(k_dummy_argc, {});

Render::Render() {
    show();
}

Render& Render::GetInstance() {
    static Render event_handler;
    return event_handler;
}

void Render::RenderAll() {
    if (isHidden()) {
        exit(EXIT_SUCCESS);
    }
    update();
    QApplication::processEvents();
}

Render::~Render() {
    app.exit(EXIT_SUCCESS);
}