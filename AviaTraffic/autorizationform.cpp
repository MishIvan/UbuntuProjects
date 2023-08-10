#include "autorizationform.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QCloseEvent>

extern QSqlDatabase m_database;
int m_currentUserID = 0;

AutorizationForm::AutorizationForm(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);


    QSqlQuery qry(m_database);
    qry.exec("select id, name,role, coalesce(password,'') passw from public.user where not closed order by name");
    while (qry.next())
    {
           User usr;
           usr.id = qry.value(0).toULongLong();
           usr.name = qry.value(1).toString();
           usr.role = qry.value(2).toInt();
           usr.password = qry.value(3).toString();
           m_users.append(usr);
    }
    int n = m_users.size();
    QStringList lst;
    for(int i = 0;i <n; i++)
    {
        lst.append(m_users.at(i).name);
    }
    m_usersList->addItems(lst);
    m_usersList->setCurrentIndex(0);
    m_pwdCount = 0;

    QStringList rlst = {"Администратор","Диспетчер полётов","Оператор оформления билетов"};
    m_roleList->addItems(rlst);
    int idx = m_usersList->currentIndex();
    m_roleList->setCurrentIndex(m_users.at(idx).role - 1);


}
// изменили пользователя - изменить роль в списке
void AutorizationForm::on_m_usersList_currentIndexChanged(int index)
{
    if(index < 0) return;
    int role = m_users.at(index).role;
    m_roleList->setCurrentIndex(role - 1);
}

// нажата ОК
void AutorizationForm::on_m_OKButton_clicked()
{
    int index = m_usersList->currentIndex();
    if(index >= 0)
    {        
        QString pwd = m_users.at(index).password;
        if(pwd.isEmpty())
        {
            QString uname = m_users.at(index).name;
            m_currentUserID = m_users.at(index).id;
            int idx = m_roleList->currentIndex();
            QString urole = m_roleList->itemText(idx);
            m_user = QString("%1 - %2").arg(uname).arg(urole);
            m_role = idx + 1;
            m_result = 1;
            close();
            return;
        }

        QString epwd = m_password->text();
        if(epwd != pwd)
        {
            QMessageBox::warning(this,"Ошибка", "Неверный пароль");
            if(++m_pwdCount >= 3)
            {
                m_result = 0;
                close();
            }
        }
        else
        {
            QString uname = m_users.at(index).name;
            m_currentUserID = m_users.at(index).id;
            int idx = m_roleList->currentIndex();
            QString urole = m_roleList->itemText(idx);
            m_user = QString("%1 - %2").arg(uname).arg(urole);
            m_role = idx + 1;
            m_result = 1;
            close();
        }
     }

}

// нажата Отмена
void AutorizationForm::on_m_cancelButton_clicked()
{
    m_result = 0;
    close();
}

