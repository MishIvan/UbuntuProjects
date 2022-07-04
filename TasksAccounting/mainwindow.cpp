#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskDialog.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setupUi(this);
    m_database = QSqlDatabase::addDatabase("QSQLITE"); //, "Data Source=.\TasksAccounting.db; Version=3;");
    QStringList co = m_database.connectionNames();
    m_database.setDatabaseName("/home/ivan/Projects/build-TasksAccounting-Desktop-Debug/TasksAccounting.db");
    if(!m_database.open())
    {
        QMessageBox::critical(this,"Ошибка", m_database.lastError().text());
    }
    bool i;
    qDebug() << m_database.tables().count();
    m_model = new QSqlTableModel;
    m_model->setTable("Tasks");
    //QSqlQuery qr(m_database);
    //qr.prepare("select * from Tasks");
//    if(!qr.exec("select * from Tasks"))
//    {
//        qDebug() << m_database.lastError().text();
//    }

    //m_model->setQuery(qr);
    m_model->select();
    qDebug() << m_model->record(0).value("Deadline").toString();
    i = m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    i = m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    i = m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание"));
    i = m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Срок"));
    i = m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Завершение"));
    i = m_model->setHeaderData(5, Qt::Horizontal, QObject::tr("Время по плану"));
    i = m_model->setHeaderData(6, Qt::Horizontal, QObject::tr("Фактическое время"));
    taskTableView->setModel(m_model);
    taskTableView->setColumnHidden(0, true);
    taskTableView->setColumnHidden(2, true);


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
    taskDialog dlg(this);
    dlg.setModal(true);
    int i = dlg.exec();
}
