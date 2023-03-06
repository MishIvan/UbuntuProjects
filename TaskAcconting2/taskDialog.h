#ifndef DIALOG_H
#define DIALOG_H

#include "ui_taskDialog.h"
//#include "timespan.h"
#include <QtSql>

class taskDialog : public QDialog, private Ui::tasksDialog
{
    Q_OBJECT
    bool m_edit; // правка записи (да) или добавление (нет)
public:
    explicit taskDialog(bool isEdit = false,QWidget *parent = nullptr);
private slots:
    void on_tasksDialog_accepted();
};

#endif // DIALOG_H
