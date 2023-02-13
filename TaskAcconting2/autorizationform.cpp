#include "autorizationform.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QCloseEvent>

extern QSqlDatabase m_database;
long userID;
QString userLogin;
User m_currentUser;

AutorizationForm::AutorizationForm(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);


    QSqlQuery qry(m_database);
    qry.exec("select id, name, role, password from users order by name");
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

}

void AutorizationForm::on_m_usersList_currentIndexChanged(int index)
{
    if(index < 0) return;
     int role = m_users.at(index).role;
     switch(role)
     {
        case 1:
             m_role->setText("Администратор"); break;
        case 2:
            m_role->setText("Руководитель проекта"); break;
        case 3:
            m_role->setText("Руководитель проекта с правами администратора"); break;
        default:
            m_role->setText("Исполнитель");
     }

}


void AutorizationForm::on_m_OKButton_clicked()
{
    int index = m_usersList->currentIndex();
    if(index >= 0)
    {
        QString epwd = m_password->text();
        QString pwd = m_users.at(index).password;
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
            m_currentUser.id = m_users.at(index).id;
            m_currentUser.name = m_users.at(index).name;
            m_currentUser.password = m_users.at(index).password;
            m_currentUser.role = m_users.at(index).role;
            m_result = 1;
            close();
        }
     }

}


void AutorizationForm::on_m_cancelButton_clicked()
{
    m_result = 0;
    close();
}

