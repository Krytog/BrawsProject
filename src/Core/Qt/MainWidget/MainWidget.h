#pragma once

#include "../../GameObject.h"
#include "../PaintTemplates/VisibleObject.h"

#include <qopenglwidget.h>
#include <QOpenGLWidget>
#include <unordered_map>

class TMainWidget : public QOpenGLWidget
{
public:
    TMainWidget(QOpenGLWidget* parent = nullptr);

    void PaintObjects();
    void AddToRender(const GameObject *game_object, IVisibleObject *vis_obj);
    void RemoveFromRender(const GameObject *vis_obj);

    ~TMainWidget() = default;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::unordered_map<const GameObject*, IVisibleObject*> objects_;
};