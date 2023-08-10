#include "ticketcarddialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

extern QSqlDatabase m_database;
void fillPassengers(QComboBox *plist,long id);
void fillFlightNumbers(QComboBox *cbox, long id);

TicketCardDialog::TicketCardDialog(long flightId,
                                   QString ticketNum, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_ticketNumber = ticketNum;
    m_passengerID = 0;
    m_flightID = flightId;
    QRect rc = m_flightList->geometry();
    m_flightList->setGeometry(rc.x(), rc.y(), 291, rc.height());
    if(m_ticketNumber.isEmpty())
    {
        fillPassengers(m_passengers, 0);
        fillFlightNumbers(m_flightList, m_flightID);
        m_flightList->setEnabled(false);
    }
    else
    {

        QSqlQuery qry(m_database);
        QString sqlText = QString("select number, passengerid, flightid, price, \
                placenumber, registered from ticket where number = '%1'")
                .arg(m_ticketNumber);

        if(qry.exec(sqlText))
        {
            if(qry.first())
            {
                m_passengerID = qry.value(1).toULongLong();
                fillPassengers(m_passengers, m_passengerID);
                m_number->setText(qry.value(0).toString());
                m_price->setText(qry.value(3).toString());
                m_place->setText(qry.value(4).toString());
                long flightid = qry.value(2).toULongLong();
                fillFlightNumbers(m_flightList, flightid);
            }
        }
        m_passengers->setEnabled(false);
        m_passport->setEnabled(false);
        m_addPassengerButton->setEnabled(false);
        m_number->setEnabled(false);

    }

}

// добавить пассажира
void TicketCardDialog::on_m_addPassengerButton_clicked()
{
    QString pname = m_passengers->currentText();
    QString passp = m_passport->toPlainText();
    QSqlQuery qry(m_database);
    QString sqlText = QString("insert into passenger(name, passportdata) values('%1','%2')")
            .arg(pname).arg(passp);
    if(qry.exec(sqlText))
    {
        fillPassengers(m_passengers, 0);
        int idx = m_passengers->findText(pname);
        if(idx >= 0)
            m_passengers->setCurrentIndex(idx);
    }
}

// нажата ОК - оформить или переоформить билет
void TicketCardDialog::on_buttonBox_accepted()
{
    QSqlQuery qry(m_database);
    QString sqlText;
    bool ok;
    float price = m_price->text().toFloat(&ok);
    if(!ok)
    {
        QMessageBox::warning(this, "Внимание!", "Ошибка при задании цены");
        setResult(QDialog::Rejected);
        return;
    }

    int place = m_place->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox::warning(this, "Внимание!", "Место должно быть целым числом");
        setResult(QDialog::Rejected);
        return;
    }

    QString num = m_number->text();
    long flightid = 0;
    if(m_ticketNumber.isEmpty())
    {
        flightid = m_flightID;
        long passid = m_passengers->currentData().toULongLong();
        sqlText = QString("insert into ticket (number, passengerid, flightid, placenumber, price, registered) \
values('%1',%2,%3,%4,%5,FALSE)")
                          .arg(num)
                          .arg(passid)
                          .arg(m_flightID)
                          .arg(place)
                          .arg(price);
    }
    else
    {
        flightid = m_flightList->currentData().toULongLong();
        sqlText = QString("update ticket set flightid = %1, placenumber = %2, price = %3 \
where number = '%4'")
                .arg(flightid)
                .arg(place)
                .arg(price)
                .arg(m_ticketNumber);
    }

    // проверка занятости места
    if(qry.exec(QString("select isplacefree(%1, %2)").arg(flightid).arg(place)))
    {
        qry.first();
        if(!qry.value(0).toBool())
        {
            QMessageBox::warning(this, "Внимание!",
            QString("На место №%1 уже оформлен билет").arg(place));
            setResult(QDialog::Rejected);
            return;
        }

    }
    else
    {
        QString s1 = qry.lastError().text();
        QMessageBox::warning(this, "Ошибка", s1);
        setResult(QDialog::Rejected);
        return;
    }

    if(!qry.exec(sqlText))
    {
        QString s1 = qry.lastError().text();
        QMessageBox::warning(this, "Ошибка внесения данных", s1);
        setResult(QDialog::Rejected);
    }
}

// выбрали пассажира из списка - показать его паспортные данные
void TicketCardDialog::on_m_passengers_currentIndexChanged(int index)
{
    long passengerid = m_passengers->itemData(index).toULongLong();
    QSqlQuery qry(m_database);
    QString sqlText = QString("select getpassportdata(%1)").arg(passengerid);
    if(qry.exec(sqlText))
    {
        if(qry.first())
            m_passport->setPlainText(qry.value(0).toString());
    }
}

