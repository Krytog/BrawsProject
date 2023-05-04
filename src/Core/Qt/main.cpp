#include <qimage.h>
#include <qnumeric.h>
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>
#include "Helpers/CommonHelpers/DrawRotatedImageHelper.h"
#include "MainWidget/MainWidget.h"
#include "PaintTemplates/AnimatedSprite.h"
#include "PaintTemplates/BasicSprite.h"
#include "../MyTime.h"
#include "PaintTemplates/VisibleObject.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* Some initialization */
    TMainWidget* main_window = new TMainWidget();
    main_window->show();

    BasicSprite* image = new BasicSprite("soome.png", {10, -20}, 100, 100);
    main_window->AddToRender(new PersistentObject(), image);

    BasicSprite* back = new BasicSprite("back.jpeg", {0, 0}, 1000, 700, IVisibleObject::RenderLayers::BaseLayer);
    main_window->AddToRender(new PersistentObject(), back);

    AnimatedSprite* guy = new AnimatedSprite("guuy.png", {-20, 0}, 300, 300, 25, 3, false);
    main_window->AddToRender(new PersistentObject(), guy);
    // guy->ChangeRenderLogic([guy](QPainter* painter){
        
    // });

    MyTime time;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }
        QApplication::processEvents();
        if (main_window->isHidden()) {
            break;
        }
        time.ResetTime();
        main_window->update();
        image->Translate({1, 1});
    }

    app.exit();
}