#include "MainWidget.h"
#include <qopenglwidget.h>
#include <QTimer>
#include <deque>

// constexpr static size_t kWindowWidth = 720;
// constexpr static size_t kWindowHeight = 540;

TMainWidget::TMainWidget(QWidget* parent): QOpenGLWidget(parent) {
    // setFixedSize(kWindowWidth, kWindowHeight);
    setAutoFillBackground(true);
    setFixedSize(720, 540);
}

void TMainWidget::AddToRender(const GameObject *game_object, IVisibleObject *vis_obj) {
    objects_[game_object] = vis_obj;
    if (!layer_to_object_.count(vis_obj->GetLayer())) {
        layer_to_object_[vis_obj->GetLayer()] = {};
    }
    layer_to_object_[vis_obj->GetLayer()].push_back(vis_obj);
}

void TMainWidget::RemoveFromRender(const GameObject *vis_obj) {
    layer_to_object_.erase(objects_.at(vis_obj)->GetLayer());
    objects_.erase(vis_obj);
}

void TMainWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.translate(360, 270); /* Fix layter, conflicts with prev impl */

    /* Translate on camera */
    if (objects_to_follow_) {
        auto coords = objects_to_follow_->GetPosition().GetCoordinates();
        painter.translate(-coords.first, coords.second);
    }

    painter.setRenderHint(QPainter::Antialiasing);

    for (const auto& [layer, vis_objects]: layer_to_object_) {
        for (const auto& obj: vis_objects) {
            obj->RenderIt(&painter);
        }
    }
    painter.end();
}

void TMainWidget::SetCameraOn(const GameObject *object) {
    objects_to_follow_ = object;
}

Position TMainWidget::GetCameraPosition() const {
    return objects_to_follow_->GetPosition();
}
