#include "changestatusdialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

extern QSqlDatabase m_database;

ChangeStatusDialog::ChangeStatusDialog(long flightId,
                                       int currentStatusId,
                                       QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_statusID = currentStatusId;
    m_flightID = flightId;
    fillStatus();
}

// заполнить данными о статусе
void ChangeStatusDialog::fillStatus()
{
   QSqlQuery qry(m_database);
   QString sqlText = QString("select id, name from status where id != %1").arg(m_statusID);
   if(qry.exec(sqlText))
   {
       while(qry.next())
       {
           m_statusList->addItem(qry.value(1).toString(),
                                 QVariant(qry.value(0).toInt()));
       }
   }
}
// Нажата ОК - изменить статус
void ChangeStatusDialog::on_buttonBox_accepted()
{
    QSqlQuery qry(m_database);
    int statusid = m_statusList->currentData().toInt();
    QString sqlText = QString("call setflightstatus(%1, %2)")
            .arg(m_flightID).arg(statusid);
    if(!qry.exec(sqlText))
    {
        QString s1 = qry.lastError().text();
        QMessageBox::warning(this, "Ошибка", s1);
        setResult(QDialog::Rejected);
    }
}

