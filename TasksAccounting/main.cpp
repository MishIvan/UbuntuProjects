#include "mainwindow.h"

#include <QApplication>

/// <summary>
/// Получить полный путь файла в папке, из которой запускается исполняемый файл программы
/// </summary>
/// <param name="fullExePath">полный путь запуска программы</param>
/// <param name="dataFileName">имя файла данных в каталоге, где расположен исполняемый файл</param>
/// <param name="fullFileName">имя файла данных в каталоге, где расположен исполняемый файл</param>
/// <returns></returns>
void GetFullPathInWD(char* fullExePath, const char* dataFileName, char* fullFileName)
{
    strcpy(fullFileName, fullExePath);
    std::string s1 = fullFileName;
    int k = s1.find_last_of('/');
    if (k == std::string::npos)
        strcpy(fullFileName, dataFileName);
    else
    {
        s1.replace(k + 1, s1.size() - 1, dataFileName);
        strcpy(fullFileName, s1.data());
    }
}
QSettings appSettings("PrivateMI", "Task Accounting");
void GetAccontingPeriod(QDate &, QDate &);
void SetAccountingPeriod(const QDate &, const QDate &);

int main(int argc, char *argv[])
{
    char buff[1024];
    GetFullPathInWD(argv[0],"TasksAccounting.db",buff);
    QString path = buff;

    // установка отчётного периода
    QDate dateBegin, dateEnd;
    GetAccontingPeriod(dateBegin, dateEnd);
    SetAccountingPeriod(dateBegin, dateEnd);

    QApplication a(argc, argv);
    MainWindow w(path);
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
