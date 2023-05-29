#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class ControlModeScreen : public QWidget {
    Q_OBJECT

public:
    ControlModeScreen(QWidget* parent = nullptr);
    ~ControlModeScreen();

private:
    QPushButton *back_button;
    QPushButton *forward_button;

    QPushButton *regular_control_mode;
    QPushButton *bot_control_mode;

    QGridLayout *control_mode_layout;

private:
    void SetButtonsDefault();

private slots:

    void ClickedBot();
    void ClickedRegular();

    void ClickedBack();
    void ClickedForward();
};
