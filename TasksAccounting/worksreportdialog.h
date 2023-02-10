#ifndef WORKSREPORTDIALOG_H
#define WORKSREPORTDIALOG_H

#include "ui_worksreportdialog.h"
#include <QtSql>
#include <QResizeEvent>

class worksReportDialog : public QDialog, private Ui::worksReportDialog
{
    Q_OBJECT
    QSqlQueryModel *m_model;
    QString m_sqlQuery;
    bool m_flag;
    bool m_asc;

    int m_oldWidth;
    int m_oldHeight;

    void ShowResults(int col = 0);
public:
    explicit worksReportDialog(QWidget *parent = nullptr);
    ~worksReportDialog();
private slots:
    void on_m_dateFromEdit_userDateChanged(const QDate &date);
    void on_m_dateToEdit_userDateChanged(const QDate &date);
    void on_sectionColumnClicked(int);
protected:
    void resizeEvent(QResizeEvent *evt) override;

};

#endif // WORKSREPORTDIALOG_H
