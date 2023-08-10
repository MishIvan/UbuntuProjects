#ifndef TICKETDIALOG_H
#define TICKETDIALOG_H

#include "ui_ticketdialog.h"
#include <QSqlQueryModel>

class TicketDialog : public QDialog, private Ui::TicketDialog
{
    Q_OBJECT
    QSqlQueryModel *m_model;
    long m_flightID;
    void setData();
    void arrangeView();
public:
    explicit TicketDialog(long flightId, QWidget *parent = nullptr);
    ~TicketDialog();
private slots:
    void on_m_createTicket_clicked();
    void on_m_registerButton_clicked();
    void on_m_moveButton_clicked();
    void on_m_ticketList_activated(const QModelIndex &index);
    void on_m_ticketList_clicked(const QModelIndex &index);
};

#endif // TICKETDIALOG_H
