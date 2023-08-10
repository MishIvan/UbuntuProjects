#ifndef CHANGESTATUSDIALOG_H
#define CHANGESTATUSDIALOG_H

#include "ui_changestatusdialog.h"

class ChangeStatusDialog : public QDialog, private Ui::ChangeStatusDialog
{
    Q_OBJECT
    int m_statusID;
    long m_flightID;
    void fillStatus();
public:
    explicit ChangeStatusDialog(long flightId, int currentStatusId, QWidget *parent = nullptr);
private slots:
    void on_buttonBox_accepted();
};

#endif // CHANGESTATUSDIALOG_H
