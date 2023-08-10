#include "flightdialog.h"
#include <QDateTime>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql/QSqlError>

extern QSqlDatabase m_database;
void fillTowns(QComboBox * town,int id);

FlightDialog::FlightDialog(long id,QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_id = id;
    if(m_id > 0)
    {
        QSqlQuery qry(m_database);
        QString sqlText = QString("select id, number, departure, places, \
            statusid,fromid, toid, flighttime from flight where id = %1 ").arg(m_id);
        if(qry.exec(sqlText))
        {
            if(qry.first())
            {
                m_number->setText(qry.value(1).toString());
                m_timeDeparture->setDateTime(qry.value(2).toDateTime());
                QString sfltime = qry.value(7).toString();
                QTime tm = QTime::fromString(sfltime,"hh:mm:ss");
                m_flightTime->setTime(tm);
                m_placeCount->setText(qry.value(3).toString());

                int fromid = qry.value(5).toInt();
                fillTowns(m_from, fromid);

                int toid = qry.value(6).toInt();
                fillTowns(m_to, toid);
            }
        }
    }
    else
    {
        QDateTime now = QDateTime::currentDateTime();
        m_timeArrival->setDateTime(now);
        m_timeDeparture->setDateTime(now.addSecs(3600));
        m_placeCount->setText("50");
        m_flightTime->setTime(QTime(1,0));

        fillTowns(m_from, 0);
        fillTowns(m_to, 0);
    }
}
// изменилось время полёта - пересчёт количества свободных мест
void FlightDialog::on_m_placeCount_textChanged(const QString &arg1)
{
   bool ok;
   int count = arg1.toInt(&ok);
   if(!ok)
   {
       QMessageBox::warning(this, "Внимание!", "Ошибка ввода количества мест");
       return;
   }

   if(m_id > 0)
   {
       QSqlQuery qry(m_database);
       QString sqlText = QString("select freeplaces(%1,%2)").arg(m_id).arg(count);
       if(qry.exec(sqlText))
       {
           if(qry.first())
           {
              int freeplaces = qry.value(0).toInt();
              m_freePlaces->setText(QString("Количество свободных мест: %1").arg(freeplaces));
           }
       }
   }
   else
   {
        m_freePlaces->setText(QString("Количество свободных мест: %1").arg(count));
   }

}

// Изменилось время полёта - изменить время прибытия
void FlightDialog::on_m_flightTime_userTimeChanged(const QTime &time)
{
    QDateTime dt = m_timeDeparture->dateTime();
    QDateTime dt1 = dt.addSecs(time.hour()*3600 + time.minute()*60);
    m_timeArrival->setDateTime(dt1);
}

// Изменилось время отправления - изменить время прибытия
void FlightDialog::on_m_timeDeparture_dateTimeChanged(const QDateTime &dateTime)
{
    QTime tm = m_flightTime->time();
    m_timeArrival->setDateTime(dateTime.addSecs(tm.hour()*3600 + tm.minute()*60));
}

// нажали ОК - новая запись или правка существующей записи
void FlightDialog::on_m_buttonBox_accepted()
{
    QString number = m_number->text();
    QDateTime departure = m_timeDeparture->dateTime();
    QString sdeparture = departure.toString("yyyy-MM-dd hh:mm:ss");
    QTime tm = m_flightTime->time();
    QString stm = tm.toString("hh:mm:ss");
    bool ok;
    int placecount = m_placeCount->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox::warning(this, "Внимание!", "Ошибка задания количества мест");
        setResult(QDialog::Rejected);
        return;
    }
    int idfrom = m_from->currentData().toInt();
    int idto = m_to->currentData().toInt();

    QString sqlText;
    QSqlQuery qry(m_database);
    if(m_id > 0)
    {
        sqlText = QString("update flight set number = '%1', departure = '%2', ")
                .arg(number).arg(sdeparture);
        sqlText += QString("places = %1, fromid = %2, toid = %3, flighttime = '%4' where id = %5")
                .arg(placecount).arg(idfrom).arg(idto).arg(stm).arg(m_id);

    }
    else
    {
        sqlText = "insert into flight(number,departure, places, statusid, fromid, toid, flighttime) ";
        sqlText += QString("values('%1','%2',%3,1,%4,%5,'%6')")
                .arg(number).arg(sdeparture).arg(placecount).arg(idfrom).arg(idto).arg(stm);
    }

    if(!qry.exec(sqlText))
    {
        QString s1 = qry.lastError().text();
        QMessageBox::warning(this, "Внимание!", QString("Ошибка внесения данных:\n %1").arg(s1));
        setResult(QDialog::Rejected);
    }
}

