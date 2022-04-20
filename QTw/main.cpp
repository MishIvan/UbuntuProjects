#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;    
    w.setWindowTitle("Первое приложение Qt Widgets");
    w.setWindowIcon(QIcon(":/images/MarkLabel.ico"));

    QPushButton *btn = w.findChild<QPushButton *>("closeButton");
    btn->setText("Закрыть окно");
    btn->setToolTip("Закрыть приложение");

    QLabel *pic = w.findChild<QLabel *>("picWidget");
    pic->setPixmap(QPixmap(":/images/Arrow_up.png"));
    w.show();
    return a.exec();
}
