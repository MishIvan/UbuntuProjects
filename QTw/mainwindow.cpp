#include "mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui = new Ui::MainWindow;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Initialize()
{
    setWindowTitle("Первое приложение Qt Widgets");
    setWindowIcon(QIcon(":/images/MarkLabel.ico"));

    QPushButton *btn = findChild<QPushButton *>("closeButton");
    btn->setText("Закрыть окно");
    btn->setToolTip("Закрыть приложение");

    QLabel *pic = findChild<QLabel *>("picWidget");
    pic->setPixmap(QPixmap(":/images/Arrow_up.png"));
}


void MainWindow::on_closeButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Выберите файл", "~", "Text Files (*.txt);Office Files (*.pdf *.djvu)");
}

