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

MainWindow::MainWindow(QString pathToData, QWidget *parent)
    : QMainWindow(parent)

{
    setupUi(this);
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    setDatabase(pathToData);

    qDebug() << m_database.tables().count();
    m_model = new QSqlTableModel(nullptr, m_database);
    m_model->setTable("Tasks");
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

}

MainWindow::~MainWindow()
{
    m_database.close();
    delete m_model;
    delete m_timerDialog;
}

// открыть базу данных, если её нет, то создать новуюбазу ланных
void MainWindow::setDatabase(QString pathToData)
{
    if(QFile::exists(pathToData))
    {
        m_database.setDatabaseName(pathToData);
         if(!m_database.open())
         {
              QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
          }
         m_pathToDB = pathToData;
    }
    else
    {
        m_pathToDB = "TasksAccounting.db";
        m_database.setDatabaseName(m_pathToDB);
        if(!m_database.open())
        {
             QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
         }
            QFile file(":/texts/dbcreate.sql");
            if(file.open(QIODevice::ReadOnly))
            {

                QTextStream stream(&file);
                QString sqlText = stream.readAll();
                file.close();
                QStringList qrylist = sqlText.split(';');
                int n = qrylist.size() -1;
                for(int i=0; i < n; i++)
                {
                    QSqlQuery qry(m_database);
                    qry.exec(qrylist.at(i));
                }

            }
    }

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
    QFile file(":/texts/withSQL.sql");
    QString sqlText;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        sqlText = stream.readAll();
        file.close();
    }
    worksReportDialog dlg(m_database, sqlText, this);
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
        if(qr.size() > 0)
        {
            QMessageBox::warning(this,"Ошибка", "Нельзя удалять задачу для которой имеютсмя работы");
            return;
        }

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
    if(dlg.exec() == QDialog::Accepted)
    {
        QString now, s1;
        m_filterFlag = dlg.filterFlag();
        switch(m_filterFlag)
        {
            case TaskFilter::ALL:
                m_model->setFilter("ID>0");
                m_model->select();
                setWindowTitle("Учёт задач - все задачи");
                taskTableView->resizeRowsToContents();
            break;
            case TaskFilter::DONE:
                now = QDate::currentDate().toString(Qt::ISODate);
                s1 = QString("FulfillmentDate<='%1'").arg(now);
                m_model->setFilter(s1);
                m_model->select();
                setWindowTitle("Учёт задач - завершённые задачи");
                taskTableView->resizeRowsToContents();
            break;
            case TaskFilter::INFINITE:
                m_model->setFilter("FulfillmentDate = '9999-12-31'");
                m_model->select();
                setWindowTitle("Учёт задач - задачи без срока");
                taskTableView->resizeRowsToContents();
            break;
            case TaskFilter::EXPIRED:
                now = QDate::currentDate().toString(Qt::ISODate);
                m_model->setFilter("Deadline<FulfillmentDate and FulfillmentDate !='9999-12-31'");
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

// сделать резервную копию БД
void MainWindow::on_action_make_copy_triggered()
{
   QFile db(m_pathToDB);
   db.copy(m_pathToDB.replace(QString("db"),QString("dbbak")));
}
