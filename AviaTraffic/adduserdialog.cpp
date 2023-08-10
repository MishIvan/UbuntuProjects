#include "adduserdialog.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

extern QSqlDatabase m_database;
AddUserDialog::AddUserDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QStringList rlst = {"Администратор","Диспетчер полётов","Оператор оформления билетов"};
    m_roleList->addItems(rlst);

    m_password->setText("123456");
}

void AddUserDialog::on_buttonBox_accepted()
{
    QString uname = m_userName->text();
    if(uname.isEmpty())
    {
        QMessageBox::warning(this, "Внимание", "ФИО пользователя не должно быть пустым");
    }
    QString pwd = m_password->text();
    int idx = m_roleList->currentIndex() + 1;

    QSqlQuery qry(m_database);
    QString sqlText;
    if(pwd.isEmpty())
        sqlText = QString("insert into public.user(name, role, closed) \
values ('%1', %2, FALSE)").arg(uname).arg(idx);
    else
        sqlText = QString("insert into public.user(name, role, password, closed) \
values ('%1', %2, '%3', FALSE)").arg(uname).arg(idx).arg(pwd);
    if(!qry.exec(sqlText))
    {
       QString s1 = qry.lastError().text();
       QMessageBox::warning(this, "Ошибка", s1);
       setResult(QDialog::Rejected);
    }
}

