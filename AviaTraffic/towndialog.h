#ifndef TOWNDIALOG_H
#define TOWNDIALOG_H

#include "ui_towndialog.h"

class TownDialog : public QDialog, private Ui::TownDialog
{
    Q_OBJECT

public:
    explicit TownDialog(QWidget *parent = nullptr);
private slots:
    void on_buttonBox_accepted();
};

#endif // TOWNDIALOG_H
