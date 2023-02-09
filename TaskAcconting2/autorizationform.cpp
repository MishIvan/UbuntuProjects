#include "autorizationform.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

bool readIniData();
QSqlDatabase m_database;
extern QString DatabaseName;
extern QString UserName;
extern QString Password;
extern QString Host;
extern int Port;
long userID;
QString userLogin;


AutorizationForm::AutorizationForm(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    qDebug() << QSqlDatabase::drivers();
    readIniData();
    m_database = QSqlDatabase::addDatabase("QPSQL");
    m_database.setHostName(Host);
    m_database.setDatabaseName(DatabaseName);
    m_database.setUserName(UserName);
    m_database.setPassword(Password);
    m_database.setPort(Port);
    m_database.setConnectOptions();

    if(m_database.open())
    {
        qDebug() << "Connection established!";
    }
    else
    {
        QString s1 = m_database.lastError().text();
        QString se = m_database.lastError().nativeErrorCode();
        qDebug() << s1;
        QMessageBox::critical(this, "Ошибка", QString("Ошибка: %1").arg(s1));
        setResult(QDialog::Rejected);
        return;
    }

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

