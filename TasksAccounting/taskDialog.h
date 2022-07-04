#ifndef DIALOG_H
#define DIALOG_H

#include "ui_taskDialog.h"
#include "timespan.h"
struct Task
{
    QString m_name;
    QString m_content;


};

class taskDialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:
    explicit taskDialog(QWidget *parent = nullptr);
};

#endif // DIALOG_H
