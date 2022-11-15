#include "workrecdialog.h"
#include "timespan.h"

extern QTime m_workTime;

workRecDialog::workRecDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void workRecDialog::on_workRecDialog_accepted()
{
    m_contents = m_contentEdit->toPlainText();
    m_date = m_dateEdit->date();
    m_time = m_timeEdit->text();
}

// начальное заполнени полей диалога
void workRecDialog::Initialize()
{
    m_contentEdit->setPlainText(m_contents);
    m_dateEdit->setDate(m_date);
    m_timeEdit->setText(m_time);
}

// внести время по таймеру
void workRecDialog::on_m_fromTimerButton_clicked()
{
    TimeSpan ts(m_workTime);
    m_timeEdit->setText(ts.toString());
}

// добавить время по таймеру
void workRecDialog::on_m_addFromTimerButton_clicked()
{
    TimeSpan ts(m_workTime);
    QString scurrTime = m_timeEdit->text();
    TimeSpan ttimer;
    if(TimeSpan::Parse(scurrTime, ttimer))
    {
        ts += ttimer;
        m_timeEdit->setText(ts.toString());
    }
}
