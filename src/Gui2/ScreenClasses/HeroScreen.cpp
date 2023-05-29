#include "HeroScreen.h"
#include "../mainwindow.h"

HeroScreen::HeroScreen(QWidget* parent) {
    back_button = new QPushButton("back");
    forward_button = new QPushButton("forward");
    
    connect(back_button, &QPushButton::clicked, this, &HeroScreen::ClickedBack);
    connect(forward_button, &QPushButton::clicked, this, &HeroScreen::ClickedForward);

    first_hero = new QPushButton("Tank");
    second_hero = new QPushButton("Mage");
    third_hero = new QPushButton("Pirate");

    connect(first_hero, &QPushButton::clicked, this, &HeroScreen::ClickedFirst);
    connect(second_hero, &QPushButton::clicked, this, &HeroScreen::ClickedSecond);
    connect(third_hero, &QPushButton::clicked, this, &HeroScreen::ClickedThird);

    hero_layout = new QGridLayout();
    hero_layout->addWidget(first_hero, 0, 0);
    hero_layout->addWidget(second_hero, 0, 1);
    hero_layout->addWidget(third_hero, 0, 2);
    hero_layout->addWidget(back_button, 1, 0);
    hero_layout->addWidget(forward_button, 1, 2);

    this->setLayout(hero_layout);
}

HeroScreen::~HeroScreen() {
    delete back_button;
    delete forward_button;

    delete first_hero;
    delete second_hero;
    delete third_hero;
    
    delete hero_layout; 
}

void HeroScreen::ClickedFirst() {
    if (!first_hero->isFlat()) {
        ResetSelection();
        MatchInfo.gamehero = GameInfo::HeroType::TANK;
        first_hero->setFlat(true);
    } else {
        ResetSelection();
    }
}

void HeroScreen::ClickedSecond() {
    if (!second_hero->isFlat()) {
        ResetSelection();
        MatchInfo.gamehero = GameInfo::HeroType::TANK;
        second_hero->setFlat(true);
    } else {
        ResetSelection();
    }
}

void HeroScreen::ClickedThird() {
    if (!third_hero->isFlat()) {
        ResetSelection();
        MatchInfo.gamehero = GameInfo::HeroType::TANK;
        third_hero->setFlat(true);
    } else {
        ResetSelection();
    }
}

void HeroScreen::ClickedBack() {
    MainWindow::GetInstance()->ChangeScreen(Screens::START);
}

void HeroScreen::ClickedForward() {
    MainWindow::GetInstance()->ChangeScreen(Screens::MODE);
}

void HeroScreen::ResetSelection() {
    MatchInfo.gamehero = GameInfo::HeroType::RANDOM;
    first_hero->setFlat(false);
    second_hero->setFlat(false); 
    third_hero->setFlat(false);  
}
