#include "mainwindow.h"
#include <QtSql>
#include <QMessageBox>

#include <QApplication>
QSqlDatabase m_database;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString progPath(argv[0]);

    // определение пути запуска (без обратного слэша на конце)
    int k = progPath.lastIndexOf('/');
    QString pathToProgram = k < 0 ?  "" : progPath.left(k);
    m_database = QSqlDatabase::addDatabase("QSQLITE", "TestDB");
    QString dbName("QtTestDB.db");
    QString pathToData = pathToProgram + '/' + dbName;
    if(!QFile::exists(pathToData))
    {
        QMessageBox::critical(nullptr,"Ошибка", "Не найден файл базы данных");
        QApplication::exit(-2);

    }
    m_database.setDatabaseName(pathToData);
    if(!m_database.open())
     {

         QString errorMsg = m_database.lastError().text();
         QMessageBox::critical(nullptr,"Ошибка", errorMsg);
         QApplication::exit(-1);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
