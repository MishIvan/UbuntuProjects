#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QMessageBox>

#include <QMainWindow>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    QBrush *m_errorBackBrush;
    QBrush *m_errorForeBrush;

    QBrush* m_succesBackBrush;
    QBrush* m_succesForeBrush;
    bool m_toFill;
    bool m_testStarted;
    void show_results();
    void setEditableAll(bool editable = true);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_populate_table_action_triggered();
    void on_exit_action_triggered();
    void on_clear_table_action_triggered();
    void on_m_multiplyTable_itemChanged(QTableWidgetItem *item);
    void on_start_test_action_triggered();
    void on_stop_test_action_triggered();
    void on_m_multiplyTable_itemSelectionChanged();
};
#endif // MAINWINDOW_H
