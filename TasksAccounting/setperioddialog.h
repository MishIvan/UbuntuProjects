#ifndef SETPERIODDIALOG_H
#define SETPERIODDIALOG_H

#include "ui_setperioddialog.h"

class setPeriodDialog : public QDialog, private Ui::setPeriodDialog
{
    Q_OBJECT

public:
    explicit setPeriodDialog(QWidget *parent = nullptr);
private slots:
    void on_buttonBox_accepted();
};

#endif // SETPERIODDIALOG_H
