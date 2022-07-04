#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QtSql/QSqlDatabase>
#include <QSqlTableModel>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addTaskButton_clicked();

private:
    QSqlDatabase m_database;
    QSqlTableModel *m_model;
};
#endif // MAINWINDOW_H
