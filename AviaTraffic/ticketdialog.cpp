#include "ticketdialog.h"
#include "ticketcarddialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>


extern QSqlDatabase m_database;
const char * QUERY_TICKET ="select ticketnumber, passengerid, passengername, \
flightid, flightnumber, price, \
placenumber, registered, regmsg, flightstatus from ticketview \
where flightid=%1 order by passengername";

TicketDialog::TicketDialog(long flightId,QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_flightID = flightId;
    m_model = new QSqlQueryModel();
    setData();
    m_ticketList->setModel(m_model);
    arrangeView();
}
TicketDialog::~TicketDialog()
{
    delete m_model;
}
// добавление данных в модель
void TicketDialog::setData()
{
    QString sqlText = QString(QUERY_TICKET).arg(m_flightID);
    m_model->clear();
    m_model->setQuery(sqlText, m_database);

    m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Номер"));
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("ИД пассажира"));
    m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("ФИО\nпассажира"));
    m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("ИД рейса"));
    m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Номер\nрейса"));
    m_model->setHeaderData(5, Qt::Horizontal, QObject::tr("Цена"));
    m_model->setHeaderData(6, Qt::Horizontal, QObject::tr("Место"));
    m_model->setHeaderData(7, Qt::Horizontal, QObject::tr("Зарегистрирован (флаг)"));
    m_model->setHeaderData(8, Qt::Horizontal, QObject::tr("Регистрация"));
    m_model->setHeaderData(9, Qt::Horizontal, QObject::tr("Статус\nрейса"));
}
void TicketDialog::arrangeView()
{
     m_ticketList->setColumnHidden(1, true);
     m_ticketList->setColumnHidden(3, true);
     m_ticketList->setColumnHidden(4, true);
     m_ticketList->setColumnHidden(7, true);
     m_ticketList->setColumnHidden(9, true);
     m_ticketList->resizeColumnsToContents();

}
// оформить билет
void TicketDialog::on_m_createTicket_clicked()
{
     TicketCardDialog dlg(m_flightID, "", this);
     dlg.setWindowTitle("Оформление билета");
     dlg.setModal(true);
     if(dlg.exec() == QDialog::Accepted)
     {
         setData();
         arrangeView();
     }
}

// зарегистрировать билет пассажира для отправления в рейс
void TicketDialog::on_m_registerButton_clicked()
{
    QSqlQuery qry(m_database);
    QItemSelectionModel *selmodel =m_ticketList->selectionModel();
    QModelIndex idx = selmodel->currentIndex();
    if(idx.isValid())
    {
        int row = idx.row();
        long idflight = m_model->index(row,3).data().toULongLong();
        QString num = m_model->index(row,0).data().toString();
        if(qry.exec(QString("update ticket set registered = TRUE where number = '%1' and flightid = %2")
                    .arg(num)
                    .arg(idflight)))
        {
            setData();
            arrangeView();
        }
        else
        {
            QString s1 = qry.lastError().text();
            QMessageBox::warning(this, "Ошибка внесения данных", s1);
        }
    }
}

// перерегистрировать пассажира на другой рейс
void TicketDialog::on_m_moveButton_clicked()
{
     QItemSelectionModel *selmodel =m_ticketList->selectionModel();
     QModelIndex idx = selmodel->currentIndex();
     if(idx.isValid())
     {
        int row = idx.row();
        long idflight = m_model->index(row,3).data().toULongLong();
        QString num = m_model->index(row,0).data().toString();
        TicketCardDialog dlg(idflight, num, this);
        dlg.setWindowTitle("Переоформление билета");
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
        {
            setData();
            arrangeView();
        }
     }
}

// выбрана запись о билете - не давать переоформлять
// и регистрировать уже зарегистрированный билет
void TicketDialog::on_m_ticketList_activated(const QModelIndex &index)
{
}


void TicketDialog::on_m_ticketList_clicked(const QModelIndex &index)
{
        if(!index.isValid()) return;
        int row = index.row();
        bool isRegistered = m_model->index(row,7).data().toBool();
        m_registerButton->setEnabled(!isRegistered);
        m_moveButton->setEnabled(!isRegistered);

}

