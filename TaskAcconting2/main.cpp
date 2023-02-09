#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QDate>

QSettings appSettings("PrivateMI", "Task Accounting 2");
void GetAccontingPeriod(QDate &, QDate &);
void SetAccountingPeriod(const QDate &, const QDate &);
QString pathToProgram; // пути запуска программы (без обратного слэша на конце)

QString DatabaseName;
QString UserName = "postgres";
QString Password;
QString Host = "localhost";
int Port = 5432;


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
    QString pathToIni = pathToProgram +"/TasksAccounting2.ini";
    QFile file(pathToIni);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
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
            return true;
        }

    }
    return false;
}
