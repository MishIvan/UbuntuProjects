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
    int m_pwdCount;
    int m_result;
    QString m_user;
    int m_role;
public:
    explicit AutorizationForm(QWidget *parent = nullptr);
    int result() { return m_result;}
    QString user() { return m_user;}
    int role() {return m_role;}
private slots:
    void on_m_usersList_currentIndexChanged(int index);
    void on_m_OKButton_clicked();
    void on_m_cancelButton_clicked();

};

#endif // AUTORIZATIONFORM_H
