#include "protocoldialog.h"

ProtocolDialog::ProtocolDialog(QString &ptext, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_text->setPlainText(ptext);
}
