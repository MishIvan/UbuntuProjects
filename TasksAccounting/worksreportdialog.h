#ifndef WORKSREPORTDIALOG_H
#define WORKSREPORTDIALOG_H

#include "ui_worksreportdialog.h"
//#include "taskreportmodel.h"
#include <QtSql>

class worksReportDialog : public QDialog, private Ui::worksReportDialog
{
    Q_OBJECT
    QSqlDatabase m_database;
    //taskReportModel *m_model;
    QSqlQueryModel *m_model;
    QString m_sqlQuery;
    bool m_flag;
    void ShowResults();
public:
    explicit worksReportDialog(QSqlDatabase database, QWidget *parent = nullptr);
    ~worksReportDialog();
private slots:
    void on_m_closeButton_clicked();
    void on_m_dateFromEdit_userDateChanged(const QDate &date);
    void on_m_dateToEdit_userDateChanged(const QDate &date);
};

#endif // WORKSREPORTDIALOG_H
