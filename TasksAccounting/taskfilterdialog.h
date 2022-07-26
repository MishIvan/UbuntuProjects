#ifndef TASKFILTERDIALOG_H
#define TASKFILTERDIALOG_H

#include "ui_taskfilterdialog.h"
enum TaskFilter
{
    ALL = 1, DONE = 2, INFINITE = 3, EXPIRED = 4
};

class taskFilterDialog : public QDialog, private Ui::taskFilterDialog
{
    Q_OBJECT
    int m_filterFlag;
public:
    explicit taskFilterDialog(QWidget *parent = nullptr);
    int filterFlag();
    void setFilterFlag(int);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // TASKFILTERDIALOG_H
