#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

QSettings appSettings("PrivateMI", "Task Accounting");
void GetAccontingPeriod(QDate &, QDate &);
void SetAccountingPeriod(const QDate &, const QDate &);
QString pathToProgram; // пути запуска программы (без обратного слэша на конце)

QSqlDatabase m_database;
bool readIniData();

int main(int argc, char *argv[])
{
    QString progPath(argv[0]);

    // определение пути запуска (без обратного слэша на конце)
    int k = progPath.lastIndexOf('/');
    pathToProgram = k < 0 ?  "" : progPath.left(k);

    // установка отчётного периода
    QDate dateBegin, dateEnd;
    GetAccontingPeriod(dateBegin, dateEnd);
    SetAccountingPeriod(dateBegin, dateEnd);

    QApplication a(argc, argv);
    if(!readIniData())
    {
        a.exit(-1);
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}

void GetAccontingPeriod(QDate &d1, QDate &d2)
{
    appSettings.beginGroup("/Settings");
    QString s1 = appSettings.value("/PeriodBegin","").toString();
    QString s2 = appSettings.value("/PeriodEnd","").toString();
    appSettings.endGroup();

    QDate today = QDate::currentDate();

    if(s1.isEmpty())
     d1 = QDate(today.year(), today.month(),1);
    else
     d1 = QDate::fromString(s1, Qt::ISODate);


    if(s2.isEmpty())
     d2 = QDate(today.year(), today.month(),today.daysInMonth());
    else
     d2 = QDate::fromString(s2, Qt::ISODate);

}

void SetAccountingPeriod(const QDate &d1, const QDate &d2)
{
    appSettings.beginGroup("/Settings");
    appSettings.setValue("/PeriodBegin", d1.toString(Qt::ISODate) );
    appSettings.setValue("/PeriodEnd", d2.toString(Qt::ISODate) );
    appSettings.endGroup();
}

// прочитать данные для соединения с БД
bool readIniData()
{
    QString pathToIni = pathToProgram +"/TasksAccounting.ini";
    QFile fileinres(":/texts/TasksAccounting.ini");
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
            QMessageBox::critical(nullptr, "Database Connection",QString("Ошибка: %1").arg(s1));
            return false;
        }

        return true;
    }

    return false;
}
