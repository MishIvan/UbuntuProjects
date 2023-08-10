#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include "ui_changepassworddialog.h"

class ChangePasswordDialog : public QDialog, private Ui::ChangePasswordDialog
{
    Q_OBJECT
    int m_userID;
public:
    explicit ChangePasswordDialog(QWidget *parent = nullptr);
private slots:
    void on_buttonBox_accepted();
};

#endif // CHANGEPASSWORDDIALOG_H
