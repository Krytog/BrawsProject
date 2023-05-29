#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class StartScreen : public QWidget {
    Q_OBJECT

public:
    StartScreen(QWidget *parent = nullptr);
    ~StartScreen();

private:
    QPushButton *create_game_button;
    QPushButton *connect_spec_game_button;
    QPushButton *connect_rand_game_button;
    QPushButton *exit_button;

    QVBoxLayout *main_v_layout;

private slots:

    void ClickedCreate();

    void ClickedConnectSpec();

    void ClickedConnectRand();
};