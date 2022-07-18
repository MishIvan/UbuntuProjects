#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "ui_aboutdialog.h"

class aboutDialog : public QDialog, private Ui::aboutDialog
{
    Q_OBJECT

public:
    explicit aboutDialog(QWidget *parent = nullptr);
private slots:
    void on_m_closeButton_clicked();
};

#endif // ABOUTDIALOG_H
