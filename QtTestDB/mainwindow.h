#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    QSqlTableModel *m_model;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_exit_action_triggered();
    void on_add_rocord_action_triggered();
    void on_edit_record_action_triggered();
    void on_delete_record_action_triggered();
};
#endif // MAINWINDOW_H
