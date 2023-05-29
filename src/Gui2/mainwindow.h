#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>

#include "GameInfo.h"
#define MatchInfo MainWindow::GetInstance()->gameinfo

enum class Screens {
    START,
    HERO,
    MODE,
    LOAD,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* GetInstance();

    void ChangeScreen(Screens screen);

    GameInfo gameinfo;

private:
    QStackedWidget* screen_stack;
};

#endif // MAINWINDOW_H
