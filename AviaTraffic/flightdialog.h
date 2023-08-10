#ifndef FLIGHTDIALOG_H
#define FLIGHTDIALOG_H

#include "ui_flightdialog.h"

class FlightDialog : public QDialog, private Ui::FlightDialog
{
    Q_OBJECT
    long m_id; // режим редактирования записи

public:
    explicit FlightDialog(long id, QWidget *parent = nullptr);
private slots:
    void on_m_placeCount_textChanged(const QString &arg1);
    void on_m_flightTime_userTimeChanged(const QTime &time);
    void on_m_timeDeparture_dateTimeChanged(const QDateTime &dateTime);
    void on_m_buttonBox_accepted();
};

#endif // FLIGHTDIALOG_H
