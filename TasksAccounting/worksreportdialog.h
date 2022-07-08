#ifndef WORKSREPORTDIALOG_H
#define WORKSREPORTDIALOG_H

#include "ui_worksreportdialog.h"
#include "taskreportmodel.h"
#include <QtSql>

class worksReportDialog : public QDialog, private Ui::worksReportDialog
{
    Q_OBJECT
    QSqlDatabase m_database;
    taskReportModel *m_model;
    QString m_sqlQuery;
    void ShowResults();
public:
    explicit worksReportDialog(QSqlDatabase database, QString query, QWidget *parent = nullptr);
    ~worksReportDialog();
private slots:
    void on_m_setTimeButton_clicked();
    void on_m_closeButton_clicked();
};

#endif // WORKSREPORTDIALOG_H
