#include "mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "ScreenClasses/StartScreen.h"
#include "ScreenClasses/HeroScreen.h"
#include "ScreenClasses/ControlModeScreen.h"
#include "ScreenClasses/LoadScreen.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    screen_stack = new QStackedWidget();

    screen_stack->addWidget(new StartScreen);
    screen_stack->addWidget(new HeroScreen);
    screen_stack->addWidget(new ControlModeScreen);
    screen_stack->addWidget(new LoadScreen);

    setCentralWidget(screen_stack);
}

MainWindow::~MainWindow()
{
}

MainWindow* MainWindow::GetInstance() {
    static MainWindow* instance = new MainWindow();
    return instance;
}

void MainWindow::ChangeScreen(Screens screen) {
    switch(screen) {
    case Screens::START:
        screen_stack->setCurrentIndex(0);
        break;
    case Screens::HERO:
        screen_stack->setCurrentIndex(1);
        break;
    case Screens::MODE:
        screen_stack->setCurrentIndex(2);
        break;
    case Screens::LOAD:
        screen_stack->setCurrentIndex(3);
        break;
    }
}
