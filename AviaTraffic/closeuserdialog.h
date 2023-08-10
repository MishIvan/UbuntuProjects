#ifndef CLOSEUSERDIALOG_H
#define CLOSEUSERDIALOG_H

#include "ui_closeuserdialog.h"

class CloseUserDialog : public QDialog, private Ui::CloseUserDialog
{
    Q_OBJECT

public:
    explicit CloseUserDialog(QWidget *parent = nullptr);
private slots:
    void on_buttonBox_accepted();
};

#endif // CLOSEUSERDIALOG_H
