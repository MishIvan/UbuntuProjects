#include "timerdialog.h"
#include "timespan.h"
#include <QMessageBox>
extern QTime m_workTime;


timerDialog::timerDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_started = false;
    m_timerID = 0;
    m_Interval = 3; // интервал таймера в секундах
    m_timeEdit->setTime(m_workTime);
}
timerDialog::~timerDialog()
{
     if(m_timerID != 0) killTimer(m_timerID);
}

void timerDialog::timerEvent(QTimerEvent *evt)
{
    if(!m_started) return;
    m_workTime = m_workTime.addSecs(m_Interval);
    m_timeEdit->setTime(m_workTime);

}

void timerDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();
}


void timerDialog::on_m_start_stopButton_clicked()
{
    if(m_started)
    {
        m_started = false;
        m_start_stopButton->setIcon(QPixmap(":/images/stop48.png"));
    }
    else
    {
        m_started = true;
        m_start_stopButton->setIcon(QPixmap(":/images/start48.png"));
        if(m_timerID == 0) m_timerID = this->startTimer(m_Interval*1000);
    }

}

void timerDialog::on_m_resetButton_clicked()
{
    if(m_timerID != 0) {        
        m_start_stopButton->setIcon(QPixmap(":/images/start48.png"));
        m_started = false;        
    }
    m_workTime =QTime(0,0,0);
    m_timeEdit->setTime(m_workTime);
}

void timerDialog::on_m_timeEdit_userTimeChanged(const QTime &time)
{
    m_workTime = time;
}


void timerDialog::on_m_calcButton_clicked()
{
    QTime tbegin = m_beginInterval->time();
    QTime tend = m_endInterval->time();
    if(tbegin > tend)
        QMessageBox::warning(this, "Ошибка", "Время начала не может быть больше времени завершения");
    else
    {
        TimeSpan ts1(tbegin, false);
        TimeSpan ts2(tend, false);
        TimeSpan tdelta = ts2 - ts1;
        m_timeEdit->setTime(QTime::fromString(tdelta.toString(true)));
    }
}

