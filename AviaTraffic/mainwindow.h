#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    QSqlQueryModel *m_model;
    int m_userRole;
    void setFightData();
    void arrangeFlightView();
public:
    MainWindow(int userRole, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_flight_add_action_triggered();
    void on_flight_edit_action_triggered();
    void on_exit_action_triggered();
    void on_tickets_view_action_triggered();
    void on_flight_status_action_triggered();
    void on_town_add_action_triggered();
    void on_add_user_action_triggered();
    void on_close_user_action_triggered();
    void on_change_password_action_triggered();
};
#endif // MAINWINDOW_H
