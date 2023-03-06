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
#include <QFileDialog>
#include "autorizationform.h"
#include "projectscarddialog.h"

QTime m_workTime;
extern QSqlDatabase m_database;
extern QString pathToProgram;
extern User m_currentUser;

long currentTaskID = 0;
long currentProjectID = 0;

// database \"task_accounting1\" does not exist
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setupUi(this);    
    // проекты
    m_projectsModel = new QSqlQueryModel;
    QString sqlText = "select id, projectname,description, deadline, fulfillmentdate, userid, chief from projectsview";
    m_projectsModel->setQuery(sqlText,m_database);

    m_projectsModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m_projectsModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    m_projectsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание"));
    m_projectsModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Время\nпо плану"));
    m_projectsModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Время\nпо факту"));
    m_projectsModel->setHeaderData(5, Qt::Horizontal, QObject::tr("ID Руководителя"));
    m_projectsModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Руководитель"));

    m_projectsTableView->setModel(m_projectsModel);
    m_projectsTableView->setColumnHidden(0, true);
    m_projectsTableView->setColumnHidden(2, true);
    m_projectsTableView->setColumnHidden(5, true);
    m_projectsTableView->setColumnWidth(1,300);
    m_projectsTableView->setColumnWidth(6,300);


    m_projectsTableView->resizeRowsToContents();
    m_projectsTableView->show();

    currentProjectID = 0;
    QItemSelectionModel *selmodel = m_projectsTableView->selectionModel();
    if(m_projectsModel->rowCount()>0)
    {
        QModelIndex idx = m_projectsModel->index(0,0);
        selmodel->setCurrentIndex(idx, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Toggle);
        QModelIndex m_projectIndex = selmodel->currentIndex();
        if(m_projectIndex.isValid())
        {
            int row = m_projectIndex.row();
            currentProjectID = m_projectsModel->data(m_projectsModel->index(row,0)).toULongLong();
        }

    }
    m_projectsTableView->setFocus();


    // задачи
    m_model = new QSqlTableModel(nullptr, m_database);
    m_model->setTable("tasks");
    m_model->setFilter(QString("projectid = %1").arg(currentProjectID));
    m_model->select();

    m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание"));
    m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Срок"));
    m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Завершение"));
    m_model->setHeaderData(5, Qt::Horizontal, QObject::tr("Время\nпо плану"));
    m_model->setHeaderData(6, Qt::Horizontal, QObject::tr("Время\nпо факту"));

    int colCount = m_model->columnCount();
    m_model->insertColumn(colCount);

    m_taskTableView->setModel(m_model);
    m_taskTableView->setColumnHidden(0, true);
    m_taskTableView->setColumnHidden(2, true);
    m_taskTableView->setColumnWidth(1, 280);

    colCount = m_model->columnCount();
    m_taskTableView->setColumnHidden(colCount - 2, true);
    m_taskTableView->setColumnHidden(colCount - 3, true);
    m_model->setHeaderData(colCount - 1, Qt::Horizontal, QObject::tr("Исполнитель"));
    m_taskTableView->setColumnWidth(colCount - 1, 300);
    m_taskTableView->resizeRowsToContents();    

    // работы по задаче
    m_worksModel = new QSqlTableModel(nullptr, m_database);
    m_worksModel->setTable("works");

    currentTaskID = 0;
    selmodel =  m_taskTableView->selectionModel();
    if(m_model->rowCount()>0)
    {
        QModelIndex idx = m_model->index(0,0);
        selmodel->setCurrentIndex(idx, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Toggle);
    }
    //m_taskTableView->setFocus();

    QModelIndex m_taskIndex = selmodel->currentIndex();
    if(m_taskIndex.isValid())
    {
        int row = m_taskIndex.row();
        currentTaskID = m_model->data(m_model->index(row,0)).toULongLong();
    }

    QString filter = QString("taskid=%1").arg(currentTaskID);
    m_worksModel->setFilter(filter);
    m_worksModel->select();

    m_worksModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m_worksModel->setHeaderData(1, Qt::Horizontal, QObject::tr("ID задачи"));
    m_worksModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание"));
    m_worksModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Дата"));
    m_worksModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Затраченное\nвремя"));

    m_worksTableView->setModel(m_worksModel);
    m_worksTableView->hideColumn(0);
    m_worksTableView->hideColumn(1);
    m_worksTableView->setColumnWidth(2, 450);
    m_worksTableView->setColumnWidth(4, 120);
    m_worksTableView->resizeRowsToContents();

    m_workTime = QTime(0,0,0);
    m_timerDialog = new timerDialog(this);
    m_oldWidth = width();
    m_oldHeight = height();
    m_filterFlag = TaskFilter::ALL;
    setWindowTitle(QString("Учёт задач - все задачи. %1").arg(m_currentUser.name));

    m_filterName = "";

    m_expDB = new ThreadExportDB(m_database);
    QObject::connect(m_expDB, SIGNAL(finished()),
                                this, SLOT(on_end_export_action()));
    statusBar()->showMessage("Данные загружены");
}

MainWindow::~MainWindow()
{
    m_database.close();

    delete m_projectsModel;
    delete m_model;
    delete m_worksModel;

    delete m_timerDialog;
    delete m_expDB;
}

void MainWindow::resizeEvent(QResizeEvent *evt)
{
    int h = evt->size().height();
    int w = evt->size().width();
    int deltax = w - m_oldWidth;
    int deltay = h - m_oldHeight;

    m_worksTableView->setGeometry(m_worksTableView->x(), m_worksTableView->y(),
                                  m_worksTableView->width() + deltax,
                                  m_worksTableView->height() + deltay);
    m_taskTableView->setGeometry(m_taskTableView->x(), m_taskTableView->y(),
                                   m_taskTableView->width() + deltax,
                                   m_taskTableView->height() + deltay);
    m_tabWidget->setGeometry(m_tabWidget->x(), m_tabWidget->y(),
                             m_tabWidget->width() + deltax,
                             m_tabWidget->height() + deltay);
    m_tabWidget->updateGeometry();
    m_worksTableView->updateGeometry();
    m_taskTableView->updateGeometry();

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

// добавить новую задачу или новую работу по задаче
void MainWindow::on_action_add_task_triggered()
{
    int idx =  m_tabWidget->currentIndex();
    if(idx == 1)
    {
        taskDialog dlg(m_model, QModelIndex(), this);
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
            m_taskTableView->resizeRowsToContents();
    }
    else if (idx == 2)
    {
        workRecDialog wrd(this);
        wrd.m_contents = "";
        wrd.m_date = QDate::currentDate();
        wrd.m_time = "00:00:00";
        wrd.Initialize();

        if(wrd.exec() == QDialog::Accepted)
        {
             int row = m_worksModel->rowCount();
             m_worksModel->insertRows(row,1);
             m_worksModel->setData(m_worksModel->index(row,1), QVariant((qlonglong)currentTaskID));
             m_worksModel->setData(m_worksModel->index(row,2), wrd.m_contents);
             m_worksModel->setData(m_worksModel->index(row,3), wrd.m_date.toString(Qt::ISODate));
             m_worksModel->setData(m_worksModel->index(row,4), wrd.m_time);
             m_worksModel->submitAll();
             m_worksModel->setFilter(QString("TaskID=%1").arg(currentTaskID));
             m_worksModel->select();
             calculateTaskTime();
        }
    }
    else if(idx == 0)
    {
        ProjectsCardDialog dlg(0, this);
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
        {
            QString q = m_projectsModel->query().executedQuery();
            m_projectsModel->setQuery(q, m_database);
        }
    }


}

// удалить задачу и все работы по ней или удалить работу по текущей задаче
void MainWindow::on_action_delete_task_triggered()
{
    if(!m_database.isOpen())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
        return;
    }
    int idx = m_tabWidget->currentIndex();
    if (idx == 0)
    {
        if(QMessageBox::question(this,tr("Предупреждение"),
                                 tr("Вы действительно хотите удалить задачу вместе с работами по ней"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            QItemSelectionModel *selmodel =  m_taskTableView->selectionModel();
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
    else if (idx == 1)
    {
        if(QMessageBox::question(this,tr("Предупреждение"),
                                 tr("Вы действительно хотите удалить выбранную работу по задаче?"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes) return;

        QItemSelectionModel *selmodel =  m_worksTableView->selectionModel();
        QModelIndex idx = selmodel->currentIndex();
        if(idx.isValid())
        {
            m_worksModel->removeRow(idx.row());
            m_worksModel->submitAll();
            m_worksModel->select();
            calculateTaskTime();
        }

    }

}

// правка параметров задачи или параметров работы по задаче
void MainWindow::on_action_edit_task_triggered()
{
    int idx = m_tabWidget->currentIndex();

    if (idx == 1)
    {
        QItemSelectionModel *selmodel =  m_taskTableView->selectionModel();
        QModelIndex idx = selmodel->currentIndex();
        if(idx.isValid())
        {
            int row = idx.row();
            currentTaskID = m_tasksModel->data(m_tasksModel->index(row,0)).toULongLong();
        }

        taskDialog dlg(this);
        dlg.setModal(true);
        dlg.exec();
    }

    else if(idx == 2)
    {
        workRecDialog wrd(this);
        QItemSelectionModel *selmodel =  m_worksTableView->selectionModel();
        QModelIndex idx = selmodel->currentIndex();
        int row = idx.row();

        wrd.m_contents = m_worksModel->data(m_worksModel->index(row,2)).toString();
        wrd.m_date = QDate::fromString(m_worksModel->data(m_worksModel->index(row,3)).toString(),
                                       Qt::ISODate);
        wrd.m_time = m_worksModel->data(m_worksModel->index(row,4)).toString();
        wrd.Initialize();
        if(wrd.exec() == QDialog::Accepted)
        {
             m_worksModel->setData(m_worksModel->index(row,2), wrd.m_contents);
             m_worksModel->setData(m_worksModel->index(row,3), wrd.m_date.toString(Qt::ISODate));
             m_worksModel->setData(m_worksModel->index(row,4), wrd.m_time);
             m_worksModel->submitAll();
             m_worksModel->select();
             calculateTaskTime();
        }

    }

    else if(idx == 0)
    {
        QItemSelectionModel *selmodel = m_projectsTableView->selectionModel();
        QModelIndex idx = selmodel->currentIndex();
        if(idx.isValid())
        {
            int row = idx.row();
            currentProjectID = m_projectsModel->data(m_projectsModel->index(row,0)).toULongLong();
        }

        ProjectsCardDialog dlg(true, this);
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
        {
            QString sqry = m_projectsModel->query().executedQuery();
            m_projectsModel->setQuery(sqry);
        }

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
                setWindowTitle(QString("Учёт задач - все задачи. %1").arg(m_currentUser.name));
                m_taskTableView->resizeRowsToContents();
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
                setWindowTitle(QString("Учёт задач - завершённые задачи. %1").arg(m_currentUser.name));
                m_taskTableView->resizeRowsToContents();
            break;
            case TaskFilter::INFINITE:
                s1 = QString("fulfillmentdate = '9999-12-31'");
                if(!m_filterName.isEmpty())
                {
                    s1 += QString(" and Name like'%1'").arg(QString("%")+m_filterName+QString("%"));
                }
                m_model->setFilter(s1);
                m_model->select();
                setWindowTitle(QString("Учёт задач - задачи без срока. %1").arg(m_currentUser.name));
                m_taskTableView->resizeRowsToContents();
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
                setWindowTitle(QString("Учёт задач - просроченные задачи. %1").arg(m_currentUser.name));
                m_taskTableView->resizeRowsToContents();
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
    if(m_tabWidget->currentIndex() != 0) return;
    QItemSelectionModel *selmodel =  m_taskTableView->selectionModel();
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
    {
        QString dbName("TasksAccounting.db");
        QString pathToData = pathToProgram + '/' + dbName;

        QString dbfileName = QFileDialog::getSaveFileName(this,
                                                 QString("Имя файла БД SQLite"),
                                                  pathToData,
                                                  "Файлы БД SQLite (*.db)");
        if(!dbfileName.isEmpty())
        {
            m_expDB->setDbsqliteName(dbfileName);
            m_expDB->start();
        }
    }
    else
        statusBar()->showMessage(QString("Повторите попытку позже. Производится экспорт в SQLite."));
}

// слот, обрабатывающий сигнал завершения экспорта
 void MainWindow::on_end_export_action()
 {
        QString Msg = m_expDB->errorMsg();
        if(!Msg.isEmpty())
            statusBar()->showMessage(QString("Ошибка: %1").arg(Msg));
        else
            statusBar()->showMessage(QString("Экспорт в SQLite завершён"));
 }

// переход между закладками Задачи и Работы
void MainWindow::on_m_tabWidget_currentChanged(int index)
{
    // перешли на закладку задач
    if(index == 1)
    {
        QItemSelectionModel *selmodel =  m_projectsTableView->selectionModel();
        QModelIndex m_projectIndex = selmodel->currentIndex();
        currentProjectID = 0;
        if(m_projectIndex.isValid())
        {
            int row = m_projectIndex.row();
            currentProjectID = m_projectsModel->data(m_projectsModel->index(0,row)).toULongLong();
            QString pName =  m_projectsModel->data(m_projectsModel->index(row,1)).toString();
            statusBar()->showMessage(QString("Работы по проекту \"%1\"").arg(pName));
        }
        else
            statusBar()->showMessage("Проект не выбран");

        QString filter = QString("projectid=%1").arg(currentProjectID);
        m_model->setFilter(filter);
        //m_model->submitAll();
        m_taskTableView->resizeRowsToContents();
        m_taskTableView->setFocus();
    }
    // першли на закладку работ
    if(index == 2)
    {
        QItemSelectionModel *selmodel =  m_taskTableView->selectionModel();
        QModelIndex m_taskIndex = selmodel->currentIndex();
        currentTaskID = 0;
        if(m_taskIndex.isValid())
        {
            int row = m_taskIndex.row();
            currentTaskID = m_model->data(m_model->index(row,0)).toULongLong();
            QString tName =  m_model->data(m_model->index(row,1)).toString();
            statusBar()->showMessage(QString("Работы по задаче \"%1\"").arg(tName));
        }
        else
            statusBar()->showMessage("Задача не выбрана");

        QString filter = QString("taskid=%1").arg(currentTaskID);
        m_worksModel->setFilter(filter);
        m_worksModel->submitAll();
        m_worksTableView->resizeRowsToContents();
        m_worksTableView->setFocus();
    }
    else
    {
        m_taskTableView->setFocus();
        statusBar()->clearMessage();    
    }
}

// рассчитать суммарное время, затраченное на выполнение задачи
void MainWindow::calculateTaskTime()
{
    QModelIndex m_taskIndex = m_taskTableView->selectionModel()->currentIndex();
    int row = m_taskIndex.row();

    if(m_worksModel->rowCount() < 1)
    {
        m_model->setData(m_model->index(row,6), "00:00:00");
        return;
    }

    currentTaskID = m_model->data(m_model->index(row,0)).toULongLong();

    QSqlQuery qr(m_database);
    QString sqlText = QString("select sum(timespent) from works where taskid = %1").arg(currentTaskID);
    if(qr.exec(sqlText))
    {
        QString res("");
        qr.first();
            res = qr.value(0).toString();
            if(!res.isEmpty())
                m_model->setData(m_model->index(row,6), res);

    }

}
