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

    void on_action_exit_triggered();

    void on_action_time_period_triggered();

    void on_action_triggered();

    void on_action_show_timer_triggered();

    void on_action_about_triggered();

    void on_action_add_task_triggered();

    void on_action_delete_task_triggered();

    void on_action_edit_task_triggered();

    void on_action_work_list_triggered();

    void on_action_task_filter_triggered();

private:

    QSqlDatabase m_database;
    QSqlTableModel *m_model;
    void setDatabase(QString pathToData);
    timerDialog *m_timerDialog;
    int m_oldWidth;
    int m_oldHeight;
    int m_filterFlag;
protected:
    virtual void resizeEvent(QResizeEvent *);
};
#endif // MAINWINDOW_H
