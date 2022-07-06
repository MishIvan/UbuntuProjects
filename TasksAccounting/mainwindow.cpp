#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskDialog.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QString pathToData, QWidget *parent)
    : QMainWindow(parent)

{
    setupUi(this);
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    QStringList co = m_database.connectionNames();
    m_database.setDatabaseName(pathToData);
    if(!m_database.open())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
    }
    qDebug() << m_database.tables().count();
    m_model = new QSqlTableModel;
    m_model->setTable("Tasks");
    m_model->select();

    m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание"));
    m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Срок"));
    m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Завершение"));
    m_model->setHeaderData(5, Qt::Horizontal, QObject::tr("Время\nпо плану"));
    m_model->setHeaderData(6, Qt::Horizontal, QObject::tr("Фактическое\nвремя"));

    taskTableView->setModel(m_model);
    taskTableView->setColumnHidden(0, true);
    taskTableView->setColumnHidden(2, true);
    taskTableView->setColumnWidth(1, 280);

}

MainWindow::~MainWindow()
{
    m_database.close();
    delete m_model;

}


void MainWindow::on_addTaskButton_clicked()
{
    if(!m_database.isOpen())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
        return;
    }
    taskDialog dlg(m_model, QModelIndex(), this);
    dlg.setModal(true);
    dlg.exec();
}

void MainWindow::on_delTaskButton_clicked()
{
    if(!m_database.isOpen())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
        return;
    }
    QItemSelectionModel *selmodel =  taskTableView->selectionModel();
    QModelIndex idx = selmodel->currentIndex();
    long id = m_model->data( m_model->index(idx.row(),0) ).toLongLong();

    QSqlQuery qr(m_database);
    QString qText;
    qText = QString("select nullif(count(*),0) from Works where ID = %1").arg(id);
    qr.exec(qText);
    if(qr.size() > 0)
    {
        QMessageBox::warning(this,"Ошибка", "Нельзя удалять задачу для которой имеютсмя работы");
        return;
    }

    m_model->removeRow(idx.row());
    m_model->submitAll();
    m_model->select();

}

void MainWindow::on_editTaskButton_clicked()
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

void MainWindow::on_worksButton_clicked()
{

}
