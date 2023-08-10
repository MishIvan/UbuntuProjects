#include "towndialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

extern QSqlDatabase m_database;

TownDialog::TownDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void TownDialog::on_buttonBox_accepted()
{
    QString tname = m_name->text();
    if(tname.isEmpty())
    {
        QMessageBox::warning(this, "Внимание", "Наименование не должно быть пустым");
        return;
    }

    QSqlQuery qry(m_database);
    QString sqlText = QString("insert into town(name) values('%1')").arg(tname);
    if(!qry.exec(sqlText))
    {
        QString s1 = qry.lastError().text();
        QMessageBox::warning(this, "Ошибка внесения данных", s1);
    }
}

