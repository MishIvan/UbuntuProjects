#ifndef WORKSPERIODDIALOG_H
#define WORKSPERIODDIALOG_H

#include "ui_worksperioddialog.h"
#include <QtSql>
#include "taskreportmodel.h"

class worksPeriodDialog : public QDialog, private Ui::worksPeriodDialog
{
    Q_OBJECT
    QSqlDatabase m_database;
    taskReportModel *m_model;
    bool m_flag;
    void ShowResults();

public:
    explicit worksPeriodDialog(QSqlDatabase database, QWidget *parent = nullptr);
    ~worksPeriodDialog();

private slots:
    void on_m_closeButton_clicked();
    void on_m_todayCheckBox_stateChanged(int arg1);
    void on_m_periodCheckBox_stateChanged(int arg1);
    void on_m_dateFromEdit_userDateChanged(const QDate &date);
    void on_m_dateToEdit_userDateChanged(const QDate &date);
};

#endif // WORKSPERIODDIALOG_H
