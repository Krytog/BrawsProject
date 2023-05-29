#include "StartScreen.h"
#include "../mainwindow.h"

StartScreen::StartScreen(QWidget* parent) {
    create_game_button = new QPushButton("Create game");
    connect_spec_game_button = new QPushButton("Connect specific game");
    connect_rand_game_button = new QPushButton("Connect random game");
    exit_button = new QPushButton("Exit");
    
    main_v_layout = new QVBoxLayout();
    main_v_layout->addWidget(create_game_button);
    main_v_layout->addWidget(connect_spec_game_button);
    main_v_layout->addWidget(connect_rand_game_button);
    main_v_layout->addWidget(exit_button);

    this->setLayout(main_v_layout);

    connect(exit_button, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
    connect(create_game_button, &QPushButton::clicked, this, &StartScreen::ClickedCreate);
    connect(connect_spec_game_button, &QPushButton::clicked, this, &StartScreen::ClickedConnectSpec);
    connect(connect_rand_game_button, &QPushButton::clicked, this, &StartScreen::ClickedConnectRand);
}

StartScreen::~StartScreen() {
    delete create_game_button;
    delete connect_rand_game_button;
    delete connect_spec_game_button;
    delete exit_button;

    delete main_v_layout;
}

void StartScreen::ClickedCreate() {
    MatchInfo.gamemode = GameInfo::GameType::CREATE;
    MainWindow::GetInstance()->ChangeScreen(Screens::HERO);
}

void StartScreen::ClickedConnectSpec() {
    MatchInfo.gamemode = GameInfo::GameType::FIND;
    MainWindow::GetInstance()->ChangeScreen(Screens::HERO);
}

void StartScreen::ClickedConnectRand() {
    MatchInfo.gamemode = GameInfo::GameType::RANDOM;
    MainWindow::GetInstance()->ChangeScreen(Screens::HERO);
}
