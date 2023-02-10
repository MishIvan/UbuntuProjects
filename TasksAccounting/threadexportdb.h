#ifndef THREADEXPORTDB_H
#define THREADEXPORTDB_H

#include <QThread>
#include <QtSql>

class ThreadExportDB : public QThread
{
    Q_OBJECT    
    QString m_errorMsg;
    QString m_dbsqliteName;
public:
    explicit ThreadExportDB(QObject *parent = nullptr);
    void run();
    QString  errorMsg() {return m_errorMsg;}
    void setDbsqliteName(const QString & dbname) { m_dbsqliteName = dbname; }
signals:
    void finished();
};

#endif // THREADEXPORTDB_H
