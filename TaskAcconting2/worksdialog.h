#ifndef WORKSDIALOG_H
#define WORKSDIALOG_H

#include "ui_worksdialog.h"
//#include "timespan.h"
#include "workrecdialog.h"
#include <QtSql>

class worksDialog : public QDialog, private Ui::worksDialog
{
    Q_OBJECT
    QSqlTableModel *m_tasksModel;
    QSqlTableModel *m_worksModel;
    QModelIndex m_taskIndex;
    long m_taskID;
public:
    explicit worksDialog(QSqlTableModel *model, QModelIndex idx = QModelIndex(), QWidget *parent = nullptr);
    ~worksDialog();
private slots:
    void on_closeButton_clicked();
    void on_addTaskButton_clicked();
    void on_delTaskButton_clicked();
    void closeEvent(QCloseEvent *);
    void on_editTaskButton_clicked();
};

#endif // WORKSDIALOG_H
