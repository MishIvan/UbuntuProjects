#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "timerdialog.h"
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

    void on_action_time_period_triggered();

    void on_action_triggered();

    void on_action_view_all_triggered();

    void on_action_view_done_triggered();

    void on_action_view_expired_triggered();

    void on_action_view_nodeadline_triggered();

    void on_action_show_timer_triggered();

private:

    QSqlDatabase m_database;
    QSqlTableModel *m_model;
    void setDatabase(QString pathToData);
    timerDialog *m_timerDialog;
};
#endif // MAINWINDOW_H
