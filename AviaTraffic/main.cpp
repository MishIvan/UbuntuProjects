#include "mainwindow.h"

#include <QApplication>
#include <QFileDevice>
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QComboBox>
#include "qglobal.h"
#include "autorizationform.h"

QString pathToProgram;
QSqlDatabase m_database;

bool readIniData();

int main(int argc, char *argv[])
{
    QString progPath(argv[0]);

    // определение пути запуска (без обратного слэша на конце)
    int k = progPath.lastIndexOf('/');
    pathToProgram = k < 0 ?  "" : progPath.left(k);
    QApplication a(argc, argv);

    // поключение к БД
    if(!readIniData())
    {
        a.exit(-1);
        return -1;
    }

    //  авторизация
    AutorizationForm frm;
    frm.exec();
    if(frm.result() == 1)
    {
        MainWindow w(frm.role());
        w.setWindowTitle(w.windowTitle() + " " + frm.user());
        w.show();
        return a.exec();
    }
    else
    {
         a.exit(-1);
         return -1;

     }
}

// прочитать данные для соединения с БД
bool readIniData()
{
    QString pathToIni = pathToProgram +"/TasksAccounting.ini";
    QFile fileinres(":/texts/AviaTraffic.ini");
    QFile file(pathToIni);

    if(fileinres.exists() && !file.exists())
    {
        fileinres.copy(pathToIni);
        file.setPermissions(file.permissions() | QFileDevice::WriteOwner);
    }

    if(file.open(QIODevice::ReadOnly))
    {
        QString DatabaseName;
        QString UserName = "postgres";
        QString Password;
        QString Host = "localhost";
        int Port = 5432;

        QTextStream stream(&file);
        QString str;
        while(!stream.atEnd())
        {
             str = stream.readLine();
             QStringList strlst = str.split(':');
             if(strlst[0].trimmed() == "HOST")
                 Host = strlst[1].trimmed();
             else if(strlst[0].trimmed() == "PORT")
                 Port = strlst[1].trimmed().toInt();
             else if(strlst[0].trimmed() == "DATABASE")
                 DatabaseName = strlst[1].trimmed();
             else if(strlst[0].trimmed() == "USER")
                 UserName = strlst[1].trimmed();
             else if(strlst[0].trimmed() == "PASS")
                 Password = strlst[1].trimmed();
            }
        file.close();
        qDebug() << QSqlDatabase::drivers();
        m_database = QSqlDatabase::addDatabase("QPSQL");
        m_database.setHostName(Host);
        m_database.setDatabaseName(DatabaseName);
        m_database.setUserName(UserName);
        m_database.setPassword(Password);
        m_database.setPort(Port);
        m_database.setConnectOptions();

        if(m_database.open())
            qDebug() << "Connection succeeds!";
        else
        {
            QString s1 = m_database.lastError().text();
            qDebug() << s1;
            QMessageBox::critical(nullptr, "Ошибка",s1);
            return false;
        }

        return true;
    }

    return false;
}
// Заполнить список пунктов назначения
void fillTowns(QComboBox * town,int id)
{
    QSqlQuery qry(m_database);
    QString sqlText = "select id, name from town order by name";
    if(qry.exec(sqlText))
    {
        town->clear();
        while(qry.next())
        {
            int id = qry.value(0).toInt();
            QString item = qry.value(1).toString();
            town->addItem(item, QVariant(id));
        }

        if(id > 0)
        {
            int count = town->count();
            for(int i = 0; i < count; i++)
            {
                int idata = town->itemData(i).toInt();
                if(idata == id)
                {
                    town->setCurrentIndex(i);
                    return;
                }
            }

            if(count > 0)
            {
                town->setCurrentIndex(0);
            }
        }
    }
}

// заполнить список пассажиров
void fillPassengers(QComboBox *plist,long id)
{
    QSqlQuery qry(m_database);
    QString sqlText = "select id, name from passenger order by name";
    if(qry.exec(sqlText))
    {
        plist->clear();
        while(qry.next())
        {
            plist->addItem(qry.value(1).toString(),
                           QVariant(qry.value(0).toULongLong()));
        }

        if(id > 0)
        {
            int idx = plist->findData(QVariant((qulonglong)id));
            if(idx >= 0)
                plist->setCurrentIndex(idx);
        }
    }
}

// Заполнить выпадающий список номерами рейсов
void fillFlightNumbers(QComboBox *cbox, long id)
{
    QSqlQuery qry(m_database);
    QString sqlText = QString("select f.id, f.number,\
(select t1.name from town t1 where t1.id = f.fromid) fromtown,\
(select t2.name from town t2 where t2.id = f.toid) totown \
from flight f order by number");
    if(qry.exec(sqlText))
    {
        cbox->clear();
        while(qry.next())
        {
            QString fromtown = qry.value(2).toString();
            QString totown = qry.value(3).toString();
            QString num = qry.value(1).toString();
            cbox->addItem(QString("%1 %2 - %3").arg(num).arg(fromtown).arg(totown),
                           QVariant(qry.value(0).toULongLong()));
        }

        if(id > 0)
        {
            int idx = cbox->findData(QVariant((qulonglong)id));
            if(idx >= 0)
                cbox->setCurrentIndex(idx);
        }
    }
}
