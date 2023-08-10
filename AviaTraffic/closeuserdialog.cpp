#include "closeuserdialog.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

extern QSqlDatabase m_database;

CloseUserDialog::CloseUserDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    QSqlQuery qry(m_database);
    QString sqlText = QString("select id, name from public.user where not closed order by name");
    if(qry.exec(sqlText))
    {
        while(qry.next())
        {
            m_usersList->addItem(qry.value(1).toString(),
                                 QVariant(qry.value(0).toInt()));
        }
    }
}

void CloseUserDialog::on_buttonBox_accepted()
{
    QSqlQuery qry(m_database);
    int idx = m_usersList->currentData().toInt();
    QString sqlText = QString("update public.user set closed = TRUE where id = %1").arg(idx);
    if(!qry.exec(sqlText))
    {
        QString s1 = qry.lastError().text();
        QMessageBox::warning(this, "Ошибка", s1);
        setResult(QDialog::Rejected);
    }
}

