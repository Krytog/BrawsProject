#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "LoadScreen.h"
#include "../mainwindow.h"

#include "../../NormInfrastructure/Client/Agent.h"
#include "../../Game/GameClasses/Client/GameStarters/GameStarter.h"



void LoadScreen::ClickedCreateGame() {
    // creation logic

    Agent& agent = Agent::GetInstance();
    uint64_t id = agent.GetUserID();
    std::cout << "my_id" << id << std::endl;

    Character character;
    switch(MatchInfo.gamehero) {
        case GameInfo::HeroType::MAGE:
            character = Character::MAGE;
            break;

        case GameInfo::HeroType::TANK:
            character = Character::TANK;
            break;

        case GameInfo::HeroType::PIRATE:
            character = Character::PIRATE;
            break;

        default:
            character = static_cast<Character>(rand()%3);
    }

    if (MatchInfo.gamemode == GameInfo::GameType::CREATE) {
        std::cout << "lobby id " << agent.CreateGame(character) << std::endl;
    } else {
        std::cout << "wrong option" << std::endl;
    }

    while (true) {
        if (agent.ApproveGame()) {
            std::cout << "Game Approved" << std::endl;
            break;

            pid_t pid = fork();
            if (!pid) {
                auto port = agent.GetPort();
                auto user_id = agent.GetUserID();
                std::cout << "port: " << port << " " << "user_id: " << user_id << std::endl;

                if (MatchInfo.contoltype == GameInfo::ControlType::REGULAR) {
                    GameStarter::StartManualGame(port, user_id);
                } else {
                    //GameStarter::StartBotGame();
                }
            }
            waitpid(pid, NULL, 0);

        }
    }
}

LoadScreen::LoadScreen(QWidget* parent) {

    create_game= new QPushButton("Create game");
    back = new QPushButton("back");

    main_v_layout = new QVBoxLayout();
    main_v_layout->addWidget(create_game);
    main_v_layout->addWidget(back);

    this->setLayout(main_v_layout);

    connect(create_game, &QPushButton::clicked, this, &LoadScreen::ClickedCreateGame);
    connect(back, &QPushButton::clicked, this, &LoadScreen::ClickedBack);
}

LoadScreen::~LoadScreen() {
    delete create_game;
    delete back;

    delete main_v_layout;
}

void LoadScreen::ClickedBack() {
    MainWindow::GetInstance()->ChangeScreen(Screens::MODE);
}

