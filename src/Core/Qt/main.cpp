#include <qimage.h>
#include <qnumeric.h>
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent): QWidget(parent) {
        elapsed = 0;
        setFixedSize(200, 200);
    }

    virtual ~Widget() {

    }

public slots:
    void animate() {
        ++elapsed;
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter;
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QImage bbe("download.jpeg");
        painter.drawImage(QRect(0, 0, 50, 50), bbe, QRect(0, 0, 50, 50));
        painter.end();
    }

private:
    int elapsed;
};

class Window : public QWidget
{
    Q_OBJECT

public:
    Window() {
        Widget *native = new Widget(this);
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(native);
        setLayout(layout);
    }

    virtual ~Window() {

    }
private:
    
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Window window;
    window.show();
    return a.exec();
}