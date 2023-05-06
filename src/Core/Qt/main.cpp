#include <qimage.h>
#include <qnumeric.h>
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>
#include "Helpers/CommonHelpers/DrawStaticImageHelper.h"
#include "Helpers/CommonHelpers/DrawAnimatedImageHelper.h"
#include "MainWidget/MainWidget.h"
#include "MainWidget/Render.h"
#include "PaintTemplates/AnimatedSprite.h"
#include "PaintTemplates/BasicSprite.h"
#include "../MyTime.h"
#include "PaintTemplates/VisibleObject.h"
#include "MainWidget/Render.h"
#include <iostream>

int main(int argc, char *argv[])
{
    BasicSprite* image = new BasicSprite("../RecourcesForTesting/soome.png", {10, -20}, 100, 100);
    Render::GetInstance().AddToRender(new PersistentObject(), image);

    BasicSprite* back = new BasicSprite("../RecourcesForTesting/back.jpeg", {0, 0}, 1000, 700, IVisibleObject::RenderLayers::BaseLayer);
    Render::GetInstance().AddToRender(new PersistentObject(), back);

    AnimatedSprite* guy = new AnimatedSprite("../RecourcesForTesting/guuy.png", {-20, 0}, 300, 300, 25, 3);
    Render::GetInstance().AddToRender(new PersistentObject(), guy);

    guy->ChangeRenderLogic([guy](QPainter* painter){
        DrawAnimatedImageHelper(guy, guy->GetPosition(), 90).Paint(painter);
    });
    
    MyTime time;
    int counter = 0;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }

        ++counter;
        if (counter == 30) {
            BasicSprite* image = new BasicSprite("../RecourcesForTesting/soome.png", {10, -20}, 100, 100);
            Render::GetInstance().AddToRender(new PersistentObject(), image);
        }

        time.ResetTime();
        Render::GetInstance().RenderAll();
        image->Translate({1, 1});
        EventHandler::GetInstance().TryExecuteAll();
    }
}