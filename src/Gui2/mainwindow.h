#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>

#include "GameInfo.h"
#define MatchInfo MainWindow::GetInstance()->gameinfo

enum class Screens {
    START,
    HERO,
    MODE,
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
};

#endif // MAINWINDOW_H
