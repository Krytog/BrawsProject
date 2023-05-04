#pragma once

#include "../../GameObject.h"
#include "../PaintTemplates/VisibleObject.h"

#include <qopenglwidget.h>
#include <QOpenGLWidget>
#include <unordered_map>
#include <deque>

class TMainWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    TMainWidget(QWidget* parent = nullptr);

    void AddToRender(const GameObject *game_object, IVisibleObject *vis_obj);
    void RemoveFromRender(const GameObject *vis_obj);

    ~TMainWidget() = default;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::map<IVisibleObject::RenderLayers, std::deque<IVisibleObject*>> layer_to_object_;
    std::unordered_map<const GameObject*, IVisibleObject*> objects_;
};