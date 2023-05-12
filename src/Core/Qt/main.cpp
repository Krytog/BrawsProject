#include <qcursor.h>
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
#include "PaintTemplates/TextSprite.h"
#include "PaintTemplates/VisibleObject.h"
#include "MainWidget/Render.h"
#include <iostream>

int main(int argc, char *argv[])
{

    AnimatedSprite* guy = new AnimatedSprite("../RecourcesForTesting/guuy.png", {0, 0}, 300, 300, 25, 3, true, 90, IVisibleObject::RenderLayers::Objects);
    Render::GetInstance().AddToRender(new PersistentObject(), guy);

    auto image_ptr = new PersistentObject();
    BasicSprite* image = new BasicSprite("../RecourcesForTesting/soome.png", {0, 0}, 100, 100, 180);
    Render::GetInstance().AddToRender(image_ptr, image);

    BasicSprite* back = new BasicSprite("../RecourcesForTesting/back.jpeg", {0, 0}, 1000, 700, 0, IVisibleObject::RenderLayers::BackGround);
    Render::GetInstance().AddToRender(new PersistentObject(), back);

    // guy->ChangeRenderLogic([guy](Painter* painter){
    //     DrawAnimatedImageHelper(guy, guy->GetPosition(), 90).Paint(painter);
    // });

    TextSprite* text = new TextSprite("Bebra", {-100, 60}, 100, 200, 0);
    Render::GetInstance().AddToRender(new PersistentObject(), text);
    
    Render::GetInstance().SetCameraOn(image_ptr);
    
    MyTime time;
    int counter = 0;
    while (true) {
        if (time.EvaluateTime() < static_cast<double>(1) / 60) {
            continue;
        }

        ++counter;
        if (counter == 30) {
            BasicSprite* image = new BasicSprite("../RecourcesForTesting/soome.png", {0, 0}, 100, 100);
            Render::GetInstance().AddToRender(new PersistentObject(), image);
        }

        time.ResetTime();
        Render::GetInstance().RenderAll();
        // image_ptr->Translate({1, 1});
        // image->Translate({1, 1});
        image_ptr->Translate({1, 1});
        image->Rotate(5);
        std::cout << Render::GetInstance().OnMousePressed() << std::endl;
        EventHandler::GetInstance().TryExecuteAll();
    }
}