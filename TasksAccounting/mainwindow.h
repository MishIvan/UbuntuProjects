#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QtSql>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QString, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addTaskButton_clicked();

    void on_delTaskButton_clicked();

    void on_editTaskButton_clicked();

    void on_worksButton_clicked();

    void on_action_exit_triggered();

private:
    QSqlDatabase m_database;
    QSqlTableModel *m_model;
};
#endif // MAINWINDOW_H
