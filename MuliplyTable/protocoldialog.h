#ifndef PROTOCOLDIALOG_H
#define PROTOCOLDIALOG_H

#include "ui_protocoldialog.h"

class ProtocolDialog : public QDialog, private Ui::ProtocolDialog
{
    Q_OBJECT

public:
    explicit ProtocolDialog(QString &ptext, QWidget *parent = nullptr);
};

#endif // PROTOCOLDIALOG_H
