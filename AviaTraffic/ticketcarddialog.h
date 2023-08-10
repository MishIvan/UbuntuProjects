#ifndef TICKETCARDDIALOG_H
#define TICKETCARDDIALOG_H

#include "ui_ticketcarddialog.h"

class TicketCardDialog : public QDialog, private Ui::TicketCardDialog
{
    Q_OBJECT
    QString m_ticketNumber; // если передан номер билета, то переоформить билет
    long m_passengerID;
    long m_flightID;

public:
    explicit TicketCardDialog(long flightId, QString ticketNum, QWidget *parent = nullptr);
private slots:
    void on_m_addPassengerButton_clicked();
    void on_buttonBox_accepted();
    void on_m_passengers_currentIndexChanged(int index);
};

#endif // TICKETCARDDIALOG_H
