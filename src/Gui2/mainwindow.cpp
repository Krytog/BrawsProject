#include "mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "ScreenClasses/StartScreen.h"
#include "ScreenClasses/HeroScreen.h"
#include "ScreenClasses/ControlModeScreen.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new StartScreen);
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
        setCentralWidget(new StartScreen);
        break;
    case Screens::HERO:
        setCentralWidget(new HeroScreen);
        break;
    case Screens::MODE:
        setCentralWidget(new ControlModeScreen);
        break;
    }
}
