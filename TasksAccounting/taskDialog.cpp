#include "taskDialog.h"
#include "timespan.h"
#include <QMessageBox>


taskDialog::taskDialog(QSqlTableModel *model ,QModelIndex idx, QWidget *parent) :
    QDialog(parent)
{
    m_model = model;
    m_idx = idx;
    setupUi(this);
    if(idx.isValid())
    {
            int row = idx.row();
            QModelIndex currIdx = m_model->index(row,1);
            m_name->setText(m_model->data(currIdx).toString());

            currIdx = m_model->index(row,2);
            m_content->setText(m_model->data(currIdx).toString());

            currIdx = m_model->index(row,3);
            QDate d1 = QDate::fromString(m_model->data(currIdx).toString(), Qt::ISODate);
            m_datePlan->setDate(d1);

            currIdx = m_model->index(row,4);
            d1 = QDate::fromString(m_model->data(currIdx).toString(), Qt::ISODate);
            m_dateFact->setDate(d1);

            currIdx = m_model->index(row,5);
            m_planTime->setText(m_model->data(currIdx).toString());

            currIdx = m_model->index(row,6);
            m_factTime->setText(m_model->data(currIdx).toString());
        }
    else
    {
        QDate today = QDate::currentDate();
        QDate d1 = QDate(today.year(), today.month(),today.daysInMonth());
        m_datePlan->setDate(d1);
        m_dateFact->setDate(QDate::fromString("9999-12-31", Qt::ISODate));
    }

}

void taskDialog::on_tasksDialog_accepted()
{
    QString name = m_name->text();
    QString content = m_content->toPlainText();
    QDate dp = m_datePlan->date();
    QDate df = m_dateFact->date();
    QString tp = m_planTime->text();
    QString tf = m_factTime->text();
    if(name.isEmpty() || name.isNull())
    {
        QMessageBox::warning(this,"Ошибка","Наименование задачи не должно быть пустым");
        setResult(QDialog::Rejected);
        return;
    }

    if(dp.year() < 2002)
    {
        QMessageBox::warning(this,"Ошибка","Должна быть установлена плановая дата задачи");
        setResult(QDialog::Rejected);
        return;
    }

    int row = -1;
    if(!m_idx.isValid())
    {
        row = m_model->rowCount();
        m_model->insertRows(row,1);
    }
    else
        row = m_idx.row();
    m_model->setData(m_model->index(row,1), name);
    m_model->setData(m_model->index(row,2), content);
    if(df.year() > 2001)
        m_model->setData(m_model->index(row,3), dp.toString(Qt::ISODate));
    m_model->setData(m_model->index(row,4), df.toString(Qt::ISODate));
    if(tp != "00:00")
        m_model->setData(m_model->index(row,5), tp);
    if(tf == ":")  tf = "00:00";
    if(tf != "00:00")
        m_model->setData(m_model->index(row,6), tf);
    if(!m_model->submitAll())
    {
        QString msg = m_model->database().lastError().text();
        QMessageBox::warning(this,"Ошибка",QString("Данные не внесены. %1").arg(msg));
    }
    m_model->select();


}
