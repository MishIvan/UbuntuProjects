#include "autorizationform.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

extern QSqlDatabase m_database;
long userID;
QString userLogin;


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
    for(int i;i <n; i++)
    {
        lst.append(m_users.at(i).name);
    }

}

void AutorizationForm::on_m_usersList_currentIndexChanged(int index)
{
    if(index >= 0)
    {

    }
}


void AutorizationForm::on_buttonBox_accepted()
{

    close();
}


void AutorizationForm::on_buttonBox_rejected()
{
    close();
}

