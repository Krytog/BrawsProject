#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class LoadScreen : public QWidget {
    Q_OBJECT

public:
    LoadScreen(QWidget *parent = nullptr);
    ~LoadScreen();

private:
    QPushButton *back;
    QPushButton *create_game;

    QVBoxLayout *main_v_layout;

private slots:

    void ClickedCreateGame();

    void ClickedBack();
};
