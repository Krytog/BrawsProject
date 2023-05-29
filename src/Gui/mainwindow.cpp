#include "mainwindow.h"

#include "ScreenClasses/StartScreen.h"
#include "ScreenClasses/HeroScreen.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    start = new StartScreen();
    hero = new HeroScreen();

    stack = new QStackedWidget();
    
    stack->addWidget(hero);
    stack->addWidget(start);

    setCentralWidget(stack);

    stack->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
}

MainWindow* MainWindow::GetInstance() {
    static MainWindow obj;
    return &obj;
}

#include <iostream>

void MainWindow::ChangeCentralWidget(Screens screens) {
    std::cout << "this funk was called" << std::endl;
    stack->setCurrentWidget(start);
    // this->setCentralWidget(hero);
}