#include "MainWidget.h"
#include <qopenglwidget.h>

// constexpr static size_t kWindowWidth = 720;
// constexpr static size_t kWindowHeight = 540;

TMainWidget::TMainWidget(QOpenGLWidget* parent): QOpenGLWidget(parent) {
    // setFixedSize(kWindowWidth, kWindowHeight);
    setFixedSize(720, 540);
}

void TMainWidget::AddToRender(const GameObject *game_object, IVisibleObject *vis_obj) {
    objects_[game_object] = vis_obj;
}

void TMainWidget::RemoveFromRender(const GameObject *vis_obj) {
    objects_.erase(vis_obj);
}

void TMainWidget::PaintObjects() {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (const auto& [game_obj, vis_obj]: objects_) {
        vis_obj->RenderIt(&painter);
    }
    painter.end();
}

void TMainWidget::paintEvent(QPaintEvent *event) {
    // Nothing
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (const auto& [game_obj, vis_obj]: objects_) {
        vis_obj->RenderIt(&painter);
    }
    painter.end();
}