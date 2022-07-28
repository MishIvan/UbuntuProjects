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
    void ShowResults();

public:
    explicit worksPeriodDialog(QSqlDatabase database, QWidget *parent = nullptr);
    ~worksPeriodDialog();

private slots:
    void on_m_closeButton_clicked();
    void on_m_setTimeButton_clicked();
    void on_m_todayCheckBox_stateChanged(int arg1);
};

#endif // WORKSPERIODDIALOG_H
