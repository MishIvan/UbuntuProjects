#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "Helper.h"
#include "participantsmodel.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    ParticipantsModel *model;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();        

private slots:
    void on_addBunnon_clicked();

    void on_delButton_clicked();

    void on_sortButton_clicked();
    void test();

    void on_action_open_triggered();

    void on_action_save_triggered();
    
    void on_action_exit_triggered();
    
private:
};
#endif // MAINWINDOW_H
