#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class HeroScreen : public QWidget {
    Q_OBJECT

public:
    HeroScreen(QWidget *parent = nullptr);
    ~HeroScreen();

private:
    void ResetSelection();

private:
    QPushButton *back_button;
    QPushButton *forward_button;

    QPushButton *first_hero;
    QPushButton *second_hero;
    QPushButton *third_hero;

    QGridLayout *hero_layout;

private slots:

    void ClickedFirst();
    void ClickedSecond();
    void ClickedThird();

    void ClickedBack();
    void ClickedForward();
};