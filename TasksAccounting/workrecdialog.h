#ifndef WORKRECDIALOG_H
#define WORKRECDIALOG_H

#include "ui_workrecdialog.h"

class workRecDialog : public QDialog, private Ui::workRecDialog
{
    Q_OBJECT

public:
    QString m_contents;
    QDate m_date;
    QString m_time;    
    explicit workRecDialog(QWidget *parent = nullptr);
    void Initialize();
private slots:
    void on_workRecDialog_accepted();
};

#endif // WORKRECDIALOG_H
