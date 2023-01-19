#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskDialog.h"
#include "worksdialog.h"
#include "worksreportdialog.h"
#include "worksperioddialog.h"
#include "aboutdialog.h"
#include "taskfilterdialog.h"
#include "setperioddialog.h"
#include <QMessageBox>
#include <QSql>
#include <QDebug>

QTime m_workTime;
extern QString pathToProgram;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setupUi(this);    
    m_expDB = nullptr;

    qDebug() << QSqlDatabase::drivers();
    m_database = QSqlDatabase::addDatabase("QPSQL");
    m_database.setHostName("localhost");
    m_database.setDatabaseName("task_accounting");
    m_database.setUserName("ivan");
    m_database.setPassword("123456");
    m_database.setPort(5432);
    m_database.setConnectOptions();

    if(m_database.open())
    {
        qDebug() << "opened!";
    }
    else
    {
        qDebug() << m_database.lastError();
    }

    action_make_copy->setVisible(false);
    m_model = new QSqlTableModel(nullptr, m_database);
    m_model->setTable("tasks");
    m_model->select();

    m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание"));
    m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Срок"));
    m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Завершение"));
    m_model->setHeaderData(5, Qt::Horizontal, QObject::tr("Время\nпо плану"));
    m_model->setHeaderData(6, Qt::Horizontal, QObject::tr("Время\nпо факту"));

    taskTableView->setModel(m_model);
    taskTableView->setColumnHidden(0, true);
    taskTableView->setColumnHidden(2, true);
    taskTableView->setColumnWidth(1, 280);
    taskTableView->resizeRowsToContents();
    m_workTime = QTime(0,0,0);
    m_timerDialog = new timerDialog(this);
    m_oldWidth = width();
    m_oldHeight = height();
    m_filterFlag = TaskFilter::ALL;
    m_filterName = "";

    m_expDB = new ThreadExportDB(m_database);
    QObject::connect(m_expDB, SIGNAL(finished()),
                                this, SLOT(on_end_export_action()));
}

MainWindow::~MainWindow()
{
    m_database.close();
    delete m_model;
    delete m_timerDialog;
    delete m_expDB;
}

void MainWindow::resizeEvent(QResizeEvent *evt)
{
    int h = evt->size().height();
    int w = evt->size().width();
    int deltax = w - m_oldWidth;
    int deltay = h - m_oldHeight;
    taskTableView->setGeometry(taskTableView->x(), taskTableView->y(),
                                   taskTableView->width() + deltax,
                                   taskTableView->height() + deltay);
    taskTableView->updateGeometry();
    m_oldWidth = w;
    m_oldHeight = h;
}

void MainWindow::on_action_exit_triggered()
{
    QApplication::exit(0);
}

// показать задачи и работы по ней с суммарным временем работ по задачам за период
void MainWindow::on_action_time_period_triggered()
{
    /*QFile file(":/texts/withSQL.sql");
    QString sqlText;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        sqlText = stream.readAll();
        file.close();
    }*/
    worksReportDialog dlg(m_database,  this);
    dlg.exec();
}

// показать список работ по задачам с их временем выполнения за период
void MainWindow::on_action_triggered()
{
    worksPeriodDialog dlg(m_database, this);
    dlg.exec();
}

// показать или скрыть таймер
void MainWindow::on_action_show_timer_triggered()
{
    if(m_timerDialog->isHidden())
    {
        m_timerDialog->show();
        action_show_timer->setText("Скрыть таймер");
    }
    else
    {
        m_timerDialog->hide();
        action_show_timer->setText("Показать таймер");
    }
}

// отобразить диалог О программе
void MainWindow::on_action_about_triggered()
{
    aboutDialog dlg(this);
    dlg.exec();
}

// добавить новую задачу
void MainWindow::on_action_add_task_triggered()
{
    if(!m_database.isOpen())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
        return;
    }
    taskDialog dlg(m_model, QModelIndex(), this);
    dlg.setModal(true);
    if(dlg.exec() == QDialog::Accepted)
    {
        taskTableView->resizeRowToContents(m_model->rowCount() - 1);
    }
}

// удалить задачу и все работы по ней
void MainWindow::on_action_delete_task_triggered()
{
    if(!m_database.isOpen())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
        return;
    }
    if(QMessageBox::question(this,tr("Предупреждение"),
                             tr("Вы действительно хотите удалить задачу вместе с работами по ней"),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        QItemSelectionModel *selmodel =  taskTableView->selectionModel();
        QModelIndex idx = selmodel->currentIndex();
        long id = m_model->data( m_model->index(idx.row(),0) ).toLongLong();

        QSqlQuery qr(m_database);
        QString qText;
        m_database.transaction();
        qText = QString("delete from Works where TaskID = %1").arg(id);
        qr.exec(qText);
        m_model->removeRow(idx.row());
        m_database.commit();
        m_model->submitAll();
        m_model->select();
    }

}

// правка параметров задачи
void MainWindow::on_action_edit_task_triggered()
{
    if(!m_database.isOpen())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
        return;
    }
    QItemSelectionModel *selmodel =  taskTableView->selectionModel();
    QModelIndex idx = selmodel->currentIndex();

    taskDialog dlg(m_model,  idx, this);
    dlg.setModal(true);
    dlg.exec();
}

// отобразить диалог списка работ по задаче
void MainWindow::on_action_work_list_triggered()
{
    if(!m_database.isOpen())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
        return;
    }
    QItemSelectionModel *selmodel =  taskTableView->selectionModel();
    QModelIndex idx = selmodel->currentIndex();
    if(idx.isValid())
    {
        worksDialog wd(m_model,idx, this);
        wd.exec();
    }
}

// установить фильтр отображения задач
void MainWindow::on_action_task_filter_triggered()
{
    taskFilterDialog dlg(this);
    dlg.setFilterFlag(m_filterFlag);
    dlg.setFilterString(m_filterName);
    if(dlg.exec() == QDialog::Accepted)
    {
        QString now, s1;
        m_filterFlag = dlg.filterFlag();
        m_filterName = dlg.filterString();
        switch(m_filterFlag)
        {
            case TaskFilter::ALL:
                if(!m_filterName.isEmpty())
                {
                    s1 = QString("name like'%1'").arg(QString("%")+m_filterName+QString("%"));
                }
                else
                    s1 = QString("id>0");
                m_model->setFilter(s1);
                m_model->select();
                setWindowTitle("Учёт задач - все задачи");
                taskTableView->resizeRowsToContents();
            break;
            case TaskFilter::DONE:
                now = QDate::currentDate().toString(Qt::ISODate);
                s1 = QString("fulfillmentdate<='%1'").arg(now);
                if(!m_filterName.isEmpty())
                {
                    s1 += QString(" and name like'%1'").arg(QString("%")+m_filterName+QString("%"));
                }
                m_model->setFilter(s1);
                m_model->select();
                setWindowTitle("Учёт задач - завершённые задачи");
                taskTableView->resizeRowsToContents();
            break;
            case TaskFilter::INFINITE:
                s1 = QString("fulfillmentdate = '9999-12-31'");
                if(!m_filterName.isEmpty())
                {
                    s1 += QString(" and Name like'%1'").arg(QString("%")+m_filterName+QString("%"));
                }
                m_model->setFilter(s1);
                m_model->select();
                setWindowTitle("Учёт задач - задачи без срока");
                taskTableView->resizeRowsToContents();
            break;
            case TaskFilter::EXPIRED:
                now = QDate::currentDate().toString(Qt::ISODate);
                s1 = QString("deadline<fulfillmentdate and fulfillmentdate !='9999-12-31'");
                if(!m_filterName.isEmpty())
                {
                    s1 += QString(" and Name like'%1'").arg(QString("%")+m_filterName+QString("%"));
                }
                m_model->setFilter(s1);
                m_model->select();
                setWindowTitle("Учёт задач - просроченные задачи");
                taskTableView->resizeRowsToContents();
            break;
            default:
            break;
        };
    }
}

// установить отчётный период
void MainWindow::on_action_set_accounting_period_triggered()
{
    setPeriodDialog dlg(this);
    dlg.exec();
}

// установить дату завершения задачи
void MainWindow::on_action_task_finish_triggered()
{
    if(!m_database.isOpen())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
        return;
    }
    QItemSelectionModel *selmodel =  taskTableView->selectionModel();
    QModelIndex idx = selmodel->currentIndex();
    if(idx.isValid())
    {
        int row = idx.row();
        long id = m_model->data(m_model->index(row,0)).toULongLong();
        QString sqlText = QString("call set_ending_date(%1)").arg(id);
        QSqlQuery qr(m_database);
        if(qr.exec(sqlText))
            m_model->select();
    }

}

// экспорт в БД SQLite
void MainWindow::on_action_sqlite_export_triggered()
{    
    if(!m_expDB->isRunning())
        m_expDB->start();
/*    QString dbName("TasksAccounting.db");
    QString pathToData = pathToProgram + '/' + dbName;
    QSqlDatabase dbsqlite = QSqlDatabase::addDatabase("QSQLITE", "ExportDB");
    if(QFile::exists(pathToData))
    {
        dbsqlite.setDatabaseName(pathToData);
        if(!dbsqlite.open())
        {
              QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
              return;
         }
        dbsqlite.transaction();
        QSqlQuery qr(dbsqlite);
        qr.exec("delete from Works");
        qr.exec("delete from Tasks");
        dbsqlite.commit();
        m_pathToDB = pathToData;
    }
    else
    {
        m_pathToDB = dbName;
        dbsqlite.setDatabaseName(m_pathToDB);
        if(!dbsqlite.open())
        {
             QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
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
    QSqlDatabase::removeDatabase("ExportDB"); */

}

 void MainWindow::on_end_export_action()
 {
        QString Msg = m_expDB->errorMsg();
        if(!Msg.isEmpty())
            QMessageBox::critical(this,"Ошибка", Msg);
        else
            QMessageBox::information(this,"Сообщение", "Экспорт в SQLite завершён");

 }
