#include "mainwindow.h"
#include <QApplication>

QSettings appSettings("PrivateMI", "Task Accounting");
void GetAccontingPeriod(QDate &, QDate &);
void SetAccountingPeriod(const QDate &, const QDate &);
QString pathToProgram; // пути запуска программы (без обратного слэша на конце)

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
