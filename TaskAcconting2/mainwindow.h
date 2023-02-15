#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "timerdialog.h"
#include "threadexportdb.h"
#include <QtSql>
#include <QTableView>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
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

    void on_action_task_filter_triggered();

    void on_action_set_accounting_period_triggered();

    void on_action_task_finish_triggered();

    void on_action_sqlite_export_triggered();

    void on_end_export_action();

    void on_m_tabWidget_currentChanged(int index);

private:
    QSqlQueryModel *m_projectsModel;
    QSqlTableModel *m_model;
    QSqlTableModel *m_worksModel;
    timerDialog *m_timerDialog;

    int m_oldWidth;
    int m_oldHeight;
    int m_filterFlag;
    QString m_filterName;
    ThreadExportDB * m_expDB;
    void calculateTaskTime();
protected:
    virtual void resizeEvent(QResizeEvent *);
};
#endif // MAINWINDOW_H
