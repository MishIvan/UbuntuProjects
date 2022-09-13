#include "timerdialog.h"
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
        m_workTime =QTime(0,0,0);
        m_start_stopButton->setIcon(QPixmap(":/images/start48.png"));
        m_started = false;
        m_timeEdit->setTime(m_workTime);
    }
}

void timerDialog::on_m_timeEdit_userTimeChanged(const QTime &time)
{
    m_workTime = time;
}

