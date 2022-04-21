#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;    
    w.Initialize();
    w.show();
    return a.exec();
}
