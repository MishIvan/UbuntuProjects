#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include "ui_adduserdialog.h"

class AddUserDialog : public QDialog, private Ui::AddUserDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(QWidget *parent = nullptr);
private slots:
    void on_buttonBox_accepted();
};

#endif // ADDUSERDIALOG_H
