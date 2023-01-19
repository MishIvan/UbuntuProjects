#ifndef THREADEXPORTDB_H
#define THREADEXPORTDB_H

#include <QThread>
#include <QtSql>

class ThreadExportDB : public QThread
{
    Q_OBJECT
    QSqlDatabase m_database;
    QString m_errorMsg;
public:
    explicit ThreadExportDB(QSqlDatabase & ,QObject *parent = nullptr);
    void run();
    QString  errorMsg() {return m_errorMsg;}
signals:
    void finished();
};

#endif // THREADEXPORTDB_H
