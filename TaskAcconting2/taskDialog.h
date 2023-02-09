#ifndef DIALOG_H
#define DIALOG_H

#include "ui_taskDialog.h"
//#include "timespan.h"
#include <QtSql>

class taskDialog : public QDialog, private Ui::tasksDialog
{
    Q_OBJECT
    QModelIndex m_idx;
    QSqlTableModel *m_model;
public:
    explicit taskDialog(QSqlTableModel *model ,QModelIndex idx = QModelIndex(), QWidget *parent = nullptr);
private slots:
    void on_tasksDialog_accepted();
};

#endif // DIALOG_H
