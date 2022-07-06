#ifndef WORKSDIALOG_H
#define WORKSDIALOG_H

#include "ui_worksdialog.h"

class worksDialog : public QDialog, private Ui::worksDialog
{
    Q_OBJECT

public:
    explicit worksDialog(QWidget *parent = nullptr);
};

#endif // WORKSDIALOG_H
