#include "aboutdialog.h"

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void aboutDialog::on_m_closeButton_clicked()
{
    accept();
}
