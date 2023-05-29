#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QStackedWidget>

#define MatchInfo MainWindow::GetInstance()->gameinfo


enum class Screens {
    START,
    HERO,
    MODE,
};

struct GameInfo {
    enum class GameType {
        CREATE,
        FIND,
        RANDOM,
    }; 

    enum class HeroType {
        RANDOM,
        MAGE,
        TANK,
        PIRATE,
    };

    GameType gamemode = GameType::CREATE; 
    HeroType gamehero = HeroType::RANDOM;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    GameInfo gameinfo;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* GetInstance();
    void ChangeCentralWidget(Screens screens);

private:
    QWidget* start;
    QWidget* hero;

    QStackedWidget* stack;
};
#endif // MAINWINDOW_H
