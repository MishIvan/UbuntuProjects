#include "recordsdialog.h"
#include <QMessageBox>

RecordsDialog::RecordsDialog(QSqlTableModel *model, QModelIndex idx,QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_model = model;
    m_index = idx;
    if(m_index.isValid())
    {
        int row = m_index.row();

        QModelIndex idx = m_model->index(row,1);
        m_textField->setText(m_model->data(idx).toString());

        idx = m_model->index(row, 2);
        m_flagField->setText(m_model->data(idx).toString());

        idx = m_model->index(row,3);
        m_intEdit->setText(m_model->data(idx).toString());

        idx = m_model->index(row, 4);
        m_realField->setText(m_model->data(idx).toString());
    }
}

///  Нажата кнопка ОК
void RecordsDialog::on_RecordsDialog_accepted()
{
    QString textData = m_textField->text();
    QString flagData = m_flagField->text();

    bool ok;
    int intField = m_intEdit->text().toInt(&ok);
    if(!ok)
    {
        QMessageBox::warning(this, "Ошибка", "Неверный формат целочисленного поля");
        setResult(QDialog::Rejected);
        return;
    }

    double dblField = m_realField->text().toDouble(&ok);
    if(!ok)
    {
        QMessageBox::warning(this, "Ошибка", "Неверный формат вещественного поля");
        setResult(QDialog::Rejected);
        return;
    }

    int row = -1;
    if(m_index.isValid())
    {
        row = m_index.row();
    }
    else
    {
        row = m_model->rowCount();
        m_model->insertRows(row,1);
    }

    QModelIndex idx = m_model->index(row,1);
    m_model->setData(idx, QVariant(textData));

    idx = m_model->index(row,2);
    m_model->setData(idx, QVariant(flagData));

    idx = m_model->index(row,3);
    m_model->setData(idx, QVariant(intField));

    idx = m_model->index(row,4);
    m_model->setData(idx, QVariant(dblField));

    m_model->submitAll();
    m_model->select();

    setResult(QDialog::Accepted);

}

