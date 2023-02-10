#ifndef WORKSPERIODDIALOG_H
#define WORKSPERIODDIALOG_H

#include "ui_worksperioddialog.h"
#include <QtSql>
#include <QResizeEvent>

class worksPeriodDialog : public QDialog, private Ui::worksPeriodDialog
{
    Q_OBJECT    
    QSqlQueryModel *m_model;   
    bool m_flag;
    bool m_asc;

    int m_oldWidth;
    int m_oldHeight;

    void ShowResults(int col = 0);

public:
    explicit worksPeriodDialog(QWidget *parent = nullptr);
    ~worksPeriodDialog();

private slots:
    void on_m_todayCheckBox_stateChanged(int arg1);
    void on_m_periodCheckBox_stateChanged(int arg1);
    void on_m_dateFromEdit_userDateChanged(const QDate &date);
    void on_m_dateToEdit_userDateChanged(const QDate &date);
    void on_sectionColumnClicked(int);
protected:
    void resizeEvent(QResizeEvent *evt) override;
};

#endif // WORKSPERIODDIALOG_H
