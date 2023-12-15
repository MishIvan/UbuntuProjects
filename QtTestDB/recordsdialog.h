#ifndef RECORDSDIALOG_H
#define RECORDSDIALOG_H

#include "ui_recordsdialog.h"
#include <QSqlTableModel>

class RecordsDialog : public QDialog, private Ui::RecordsDialog
{
    Q_OBJECT
    QSqlTableModel *m_model;
    QModelIndex m_index;

public:
    explicit RecordsDialog(QSqlTableModel *model, QModelIndex idx = QModelIndex(), QWidget *parent = nullptr);
private slots:
    void on_RecordsDialog_accepted();
};

#endif // RECORDSDIALOG_H
