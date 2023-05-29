#include "ControlModeScreen.h"
#include "../mainwindow.h"

ControlModeScreen::ControlModeScreen(QWidget* parent) {
    back_button = new QPushButton("back");
    forward_button = new QPushButton("forward");
    
    connect(back_button, &QPushButton::clicked, this, &ControlModeScreen::ClickedBack);
    connect(forward_button, &QPushButton::clicked, this, &ControlModeScreen::ClickedForward);
 
    regular_control_mode = new QPushButton("mouse/keyboard");
    bot_control_mode = new QPushButton("bot mode");

    connect(regular_control_mode, &QPushButton::clicked, this, &ControlModeScreen::ClickedRegular);
    connect(bot_control_mode, &QPushButton::clicked, this, &ControlModeScreen::ClickedBot);
   
    control_mode_layout = new QGridLayout();
    control_mode_layout->addWidget(regular_control_mode, 0, 0);
    control_mode_layout->addWidget(bot_control_mode, 0, 1);
    control_mode_layout->addWidget(back_button, 1, 0);
    control_mode_layout->addWidget(forward_button, 1, 1);

    this->setLayout(control_mode_layout);
}

ControlModeScreen::~ControlModeScreen() {
    delete back_button;
    delete forward_button;

    delete regular_control_mode;
    delete bot_control_mode;

    delete control_mode_layout;
}

void ControlModeScreen::SetButtonsDefault() {
    regular_control_mode->setFlat(false);
    bot_control_mode->setFlat(false);
    MatchInfo.contoltype = GameInfo::ControlType::REGULAR;
}

void ControlModeScreen::ClickedBack() {

    MainWindow::GetInstance()->ChangeScreen(Screens::HERO);
}

void ControlModeScreen::ClickedForward() {
    MainWindow::GetInstance()->ChangeScreen(Screens::MODE);
}

void ControlModeScreen::ClickedBot() {
    SetButtonsDefault();
    bot_control_mode->setFlat(true);
    MatchInfo.contoltype = GameInfo::ControlType::BOT;
}

void ControlModeScreen::ClickedRegular() {
    SetButtonsDefault();
    regular_control_mode->setFlat(true);
    MatchInfo.contoltype = GameInfo::ControlType::REGULAR;
}
