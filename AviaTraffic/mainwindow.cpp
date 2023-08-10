#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flightdialog.h"
#include "ticketdialog.h"
#include "towndialog.h"
#include "changestatusdialog.h"
#include "adduserdialog.h"
#include "closeuserdialog.h"
#include "changepassworddialog.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

extern QSqlDatabase m_database;

const QString QUERY_FLIGHT = QString("select id,number,departure,arrival,\
fromid,fromtowm,toid,totowm,flighttime,\
freeplaces,statusid,status from flightview");

MainWindow::MainWindow(int userRole, QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    m_userRole = userRole;
    m_model = new QSqlQueryModel();
    setFightData();

    m_flightTableView->setModel(m_model);
    arrangeFlightView();
    m_flightTableView->show();

    // определить доступ к меню, согласно роли пользователя
    flight_add_action->setEnabled(m_userRole == 1 || m_userRole == 2);
    flight_edit_action->setEnabled(m_userRole == 1 || m_userRole == 2);
    flight_status_action->setEnabled(m_userRole == 1 || m_userRole == 2);
    town_add_action->setEnabled(m_userRole == 1 || m_userRole == 2);
    tickets_view_action->setEnabled(m_userRole == 1 || m_userRole == 3);

    add_user_action->setEnabled(m_userRole == 1);
    close_user_action->setEnabled(m_userRole == 1);

}

MainWindow::~MainWindow()
{
    if(m_database.isOpen())
        m_database.close();
    delete m_model;
}

void MainWindow::setFightData()
{
    m_model->clear();
    m_model->setQuery(QUERY_FLIGHT, m_database);

    m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Номер"));
    m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Время\nотправления"));
    m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Время\nприбытия"));
    m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("ИД откуда"));
    m_model->setHeaderData(5, Qt::Horizontal, QObject::tr("Вылет\nоткуда"));
    m_model->setHeaderData(6, Qt::Horizontal, QObject::tr("ИД куда"));
    m_model->setHeaderData(7, Qt::Horizontal, QObject::tr("Прибытие\nкуда"));
    m_model->setHeaderData(8, Qt::Horizontal, QObject::tr("Время\nполёта"));
    m_model->setHeaderData(9, Qt::Horizontal, QObject::tr("Число\nсвободных\nмест"));
    m_model->setHeaderData(10, Qt::Horizontal, QObject::tr("ИД статуса"));
    m_model->setHeaderData(11, Qt::Horizontal, QObject::tr("Статус"));

}

void MainWindow::arrangeFlightView()
{
    m_flightTableView->setColumnHidden(0, true);
    m_flightTableView->setColumnHidden(4, true);
    m_flightTableView->setColumnHidden(6, true);
    m_flightTableView->setColumnHidden(10, true);
    m_flightTableView->resizeColumnsToContents();
}

// Добавить рейс
void MainWindow::on_flight_add_action_triggered()
{
    FlightDialog dlg(0, this);
    dlg.setModal(true);
    if(dlg.exec() == QDialog::Accepted)
    {
        setFightData();
        arrangeFlightView();
    }
}

// Править параметры рейса
void MainWindow::on_flight_edit_action_triggered()
{
    QItemSelectionModel * model = m_flightTableView->selectionModel();
    QModelIndex idx = model->currentIndex();
    if(idx.isValid())
    {
        int row = idx.row();
        idx = m_model->index(row,10);
        int statusid = idx.data().toInt();
        if(statusid != 1)
        {
            QMessageBox::warning(this, "Внимание!",
            QString("Можно править параметры рейса\n\
только со статусом\"Готов к отправлению\"") );
            return;
        }
        idx = m_model->index(row,0);
        long idflight = idx.data().toULongLong();
        FlightDialog dlg(idflight, this);
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
        {
            setFightData();
            arrangeFlightView();
        }

    }
}

// завершить работу программы
void MainWindow::on_exit_action_triggered()
{
    QApplication::exit(0);
}

// показать билеты по выбранному рейсу, оформление, перенаправление, регистрация
void MainWindow::on_tickets_view_action_triggered()
{
    QItemSelectionModel * model = m_flightTableView->selectionModel();
    QModelIndex idx = model->currentIndex();
    if(idx.isValid())
    {
        int row = idx.row();
        idx = m_model->index(row,0);
        long idflight = idx.data().toULongLong();
        QString title = QString("Билеты на рейс №%1 - %2")
                .arg(m_model->index(row,1).data().toString())
                .arg(m_model->index(row,11).data().toString());
        TicketDialog dlg(idflight, this);
        dlg.setWindowTitle(title);
        dlg.setModal(true);
        dlg.exec();
    }
}

// изменить статус рейса
void MainWindow::on_flight_status_action_triggered()
{
    QItemSelectionModel * model = m_flightTableView->selectionModel();
    QModelIndex idx = model->currentIndex();
    if(idx.isValid())
    {
        int row = idx.row();
        idx = m_model->index(row, 0);
        long flightid = idx.data().toULongLong();
        idx = m_model->index(row,10);
        int currstatusid = idx.data().toInt();
        ChangeStatusDialog dlg(flightid, currstatusid, this);
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
        {
           setFightData();
           arrangeFlightView();
        }
    }
}

// добавить пункт отправления и (или) прибытия
void MainWindow::on_town_add_action_triggered()
{
    TownDialog dlg(this);
    dlg.setModal(true);
    dlg.exec();
}

// добавить пользователя
void MainWindow::on_add_user_action_triggered()
{
    AddUserDialog dlg(this);
    dlg.setModal(true);
    dlg.exec();
}

// закрыть запись пользователя
void MainWindow::on_close_user_action_triggered()
{
    CloseUserDialog dlg(this);
    dlg.setModal(true);
    dlg.exec();

}

// сменить пароль текущего пользователя
void MainWindow::on_change_password_action_triggered()
{
    ChangePasswordDialog dlg(this);
    dlg.setModal(true);
    dlg.exec();
}

