#ifndef AUTORIZATIONFORM_H
#define AUTORIZATIONFORM_H

#include "ui_autorizationform.h"

struct User
{
    long id;
    QString name;
    int role;
    QString password;
};

class AutorizationForm : public QDialog, private Ui::AutorizationForm
{
    Q_OBJECT
    QList<User> m_users;
public:
    explicit AutorizationForm(QWidget *parent = nullptr);
private slots:
    void on_m_usersList_currentIndexChanged(int index);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // AUTORIZATIONFORM_H
