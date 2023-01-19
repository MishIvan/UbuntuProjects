#include "threadexportdb.h"
extern QString pathToProgram;

ThreadExportDB::ThreadExportDB(QSqlDatabase & database, QObject *parent)
    : QThread{parent}
{
    m_database = database;
    m_errorMsg = "";
}

void ThreadExportDB::run()
{
    QString dbName("TasksAccounting.db");
    QString pathToData = pathToProgram + '/' + dbName;
    QSqlDatabase dbsqlite = QSqlDatabase::addDatabase("QSQLITE", "ExportDB");
    if(QFile::exists(pathToData))
    {
         dbsqlite.setDatabaseName(pathToData);
         if(!dbsqlite.open())
          {
              //QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
              m_errorMsg = m_database.lastError().text();
              emit finished();
              return;
         }
         dbsqlite.transaction();
         QSqlQuery qr(dbsqlite);
         qr.exec("delete from Works");
         qr.exec("delete from Tasks");
         dbsqlite.commit();
      }
      else
      {
         dbsqlite.setDatabaseName(dbName);
         if(!dbsqlite.open())
         {
            //QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
              m_errorMsg = m_database.lastError().text();
              emit finished();
              return;
         }
          QFile file(":/texts/dbcreate.sql");
          if(file.open(QIODevice::ReadOnly))
          {
            QTextStream stream(&file);
            QString sqlText = stream.readAll();
            file.close();
            QStringList qrylist = sqlText.split(';');
            QSqlQuery qry(dbsqlite);
            int n = qrylist.size() -1;
            for(int i=0; i < n; i++)
                qry.exec(qrylist.at(i));
            }
      }

        // выгрузка данных
        QSqlQuery qr(m_database);
        QString sqlText = QString("select t.id,t.name,t.content,t.deadline,t.fulfillmentdate,t.plan,t.fact from public.tasks t");
        qr.exec(sqlText);
        QList <QString> lst;
        QString str;
        while(qr.next())
        {
            str = QString("insert into Tasks (ID, Name,Content, Deadline,FulfillmentDate,Plan,Fact) values (%1,'%2','%3','%4','%5','%6','%7')")
                    .arg(qr.value(0).toLongLong())
                    .arg(qr.value(1).toString())
                    .arg(qr.value(2).toString())
                    .arg(qr.value(3).toString())
                    .arg(qr.value(4).toString())
                    .arg(qr.value(5).toString())
                    .arg(qr.value(6).toString());
            lst.append(str);
        }

        QSqlQuery qr1(m_database);
        sqlText = QString("select w.id,w.taskid,w.content,w.date,w.timespent from public.works w");
        qr1.exec(sqlText);
        while(qr1.next())
        {
            str = QString("insert into Works (ID,TaskID,Content,Date,TimeSpent) values (%1,%2,'%3','%4','%5')")
                    .arg(qr1.value(0).toLongLong())
                    .arg(qr1.value(1).toLongLong())
                    .arg(qr1.value(2).toString())
                    .arg(qr1.value(3).toString())
                    .arg(qr1.value(4).toString());
            lst.append(str);
        }

        // вставка данных
        QSqlQuery qry(dbsqlite);
        int n = lst.count();
        dbsqlite.transaction();
        for(int i =0; i < n ; i++)
            qry.exec(lst.at(i));
        dbsqlite.commit();
        QSqlDatabase::removeDatabase("ExportDB");
        m_errorMsg = "";
        emit finished();
}
