#pragma once

#include "../../GameObject.h"
#include "../../InputSystem.h"
#include "../PaintTemplates/VisibleObject.h"

#include <qopenglwidget.h>
#include <QOpenGLWidget>
#include <unordered_map>
#include <deque>
#include <list>

constexpr static size_t kWindowWidth = 1280;
constexpr static size_t kWindowHeight = 720;

class TMainWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    TMainWidget(QWidget* parent = nullptr);

    void AddToRender(const GameObject *game_object, IVisibleObject *vis_obj);
    void RemoveFromRender(const GameObject *vis_obj);

    void SetCameraOn(const GameObject *object);
    Position GetCameraPosition() const;

    InputSystem::InputTokensArray GetKeyBoardInput();
    bool OnMousePressed() const;


    ~TMainWidget() = default;

protected:
    void paintEvent(QPaintEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::map<IVisibleObject::RenderLayers, std::deque<IVisibleObject*>> layer_to_object_;
    std::unordered_map<const GameObject*, IVisibleObject*> objects_;

    const GameObject* objects_to_follow_ = nullptr;
    InputSystem::InputTokensArray key_buffer_;
    std::unordered_map<int64_t, bool> key_pressed_;
    bool mouse_pressed_ = false;
};