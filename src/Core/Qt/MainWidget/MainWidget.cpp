#include "MainWidget.h"
#include "../Painters/QtPainter.h"
#include "../../Tools/Concepts.h"

#include <qopenglwidget.h>
#include <QTimer>
#include <deque>
#include <iostream>
#include <QKeyEvent>
#include <ostream>
#include <unordered_map>

const static std::unordered_map<char, int64_t> kKeyMap = {{'A', Qt::Key_A}, {'S', Qt::Key_S},
                                                          {'D', Qt::Key_D}, {'W', Qt::Key_W},
                                                          {'Q', Qt::Key_Q}, {'E', Qt::Key_E}};

TMainWidget::TMainWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setFixedSize(kWindowWidth, kWindowHeight);
    setAutoFillBackground(true);
}

void TMainWidget::AddToRender(const GameObject* game_object, IVisibleObject* vis_obj) {
    objects_[game_object] = vis_obj;
    if (!layer_to_object_.count(vis_obj->GetLayer())) {
        layer_to_object_[vis_obj->GetLayer()] = {};
    }
    layer_to_object_[vis_obj->GetLayer()].push_back(vis_obj);
}

void TMainWidget::RemoveFromRender(const GameObject* vis_obj) {
    layer_to_object_.erase(objects_.at(vis_obj)->GetLayer());
    objects_.erase(vis_obj);
}

void TMainWidget::paintEvent(QPaintEvent* event) {
    QtPainter painter;
    painter.begin(this);
    painter.translate(kWindowWidth / 2, kWindowHeight / 2);

    /* Translate on camera */
    if (objects_to_follow_) {
        auto coords = objects_to_follow_->GetPosition().GetCoordinates();
        painter.translate(-coords.first, coords.second);
    }

    painter.setRenderHint(QPainter::Antialiasing);

    for (const auto& [layer, vis_objects] : layer_to_object_) {
        for (const auto& object : vis_objects) {
            object->RenderIt(&painter);
        }
    }

    painter.end();
}

void TMainWidget::SetCameraOn(const GameObject* object) {
    objects_to_follow_ = object;
}

Position TMainWidget::GetCameraPosition() const {
    if (objects_to_follow_) {
        return objects_to_follow_->GetPosition();
    }
    return {0, 0};
}

void TMainWidget::keyPressEvent(QKeyEvent* event) {
    key_pressed_[event->key()] = true;
    QOpenGLWidget::keyPressEvent(event);
}

void TMainWidget::keyReleaseEvent(QKeyEvent* event) {
    key_pressed_[event->key()] = false;
    QOpenGLWidget::keyPressEvent(event);
}

void TMainWidget::mouseMoveEvent(QMouseEvent* event) {
    mouse_pressed_ = (event->button() == Qt::LeftButton);
}

InputSystem::InputTokensArray TMainWidget::GetKeyBoardInput() {
    for (const auto& [symb, key] : kKeyMap) {
        if (key_pressed_[key]) {
            key_buffer_.emplace_back(InputSystem::KeyboardToken{symb});
        }
    }
    return std::move(key_buffer_);
}

bool TMainWidget::OnMousePressed() const {
    return mouse_pressed_;
}