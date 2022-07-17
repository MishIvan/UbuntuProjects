#ifndef TIMERDIALOG_H
#define TIMERDIALOG_H

#include "ui_timerdialog.h"
#include <QCloseEvent>

class timerDialog : public QDialog, private Ui::timerDialog
{
    Q_OBJECT
    bool m_started;
    int m_timerID;
    int m_Interval;
public:
    explicit timerDialog(QWidget *parent = nullptr);
    ~ timerDialog();
protected:
    virtual void timerEvent(QTimerEvent *) override;
    void closeEvent(QCloseEvent *) override;
private slots:
    void on_m_start_stopButton_clicked();
    void on_m_resetButton_clicked();
};

#endif // TIMERDIALOG_H
