#include "changepassworddialog.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

extern QSqlDatabase m_database;
extern int m_currentUserID;

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void ChangePasswordDialog::on_buttonBox_accepted()
{
    QSqlQuery qry(m_database);
    QString sqlText;
    QString newpwd =  m_newPass->text();
    QString confirmpass = m_confirmPass->text();
    if(newpwd != confirmpass)
    {
        QMessageBox::warning(this, "Ошибка", "Введённый пароль и его подтверждение не совпадают");
        setResult(QDialog::Rejected);
    }
    else
    {
        sqlText = QString("update public.user set password = '%1' where id = %2")
                .arg(newpwd)
                .arg(m_currentUserID);
        if(!qry.exec(sqlText))
        {
            QString s1 = qry.lastError().text();
            QMessageBox::warning(this, "Ошибка", s1);
            setResult(QDialog::Rejected);
        }
    }
}

