#include "taskDialog.h"
//#include "timespan.h"
#include <QMessageBox>
#include <QSqlDatabase>

extern long currentTaskID;
extern QSqlDatabase m_database;

taskDialog::taskDialog(bool isEdit, QWidget *parent) :
    QDialog(parent)
{
    m_edit = isEdit;
    if(m_edit)
    {
            QSqlQuery qry(m_database);
            QString sqlText = "select id,taskname,content,deadline,fulfillmentdate,plan,fact,executorid,executor,projectid,projectname";
            sqlText = QString("where id = %1").arg(currentTaskID);
            if(qry.exec(sqlText))
            {
                if(qry.first())
                {
                    m_name->setText(qry.value(1).toString());
                    m_content->setText(qry.value(2).toString());

                    QDate d1 = QDate::fromString(qry.value(3).toString(), Qt::ISODate);
                    m_datePlan->setDate(d1);

                    d1 = QDate::fromString(qry.value(4).toString(), Qt::ISODate);
                    m_dateFact->setDate(d1);

                    m_planTime->setText(qry.value(5).toString());

                    m_factTime->setText(qry.value(6).toString());

                }
            }


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

    QSqlQuery qry(m_database);
    if(m_edit)
    {
        sqlText = "insert "
    }
    else
    {
    }
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


}


