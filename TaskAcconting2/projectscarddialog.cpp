#include "projectscarddialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include "autorizationform.h"

extern QSqlDatabase m_database;
extern User m_currentUser;
extern long currentProjectID;
long getUserByName(const QString &uname);


ProjectsCardDialog::ProjectsCardDialog(bool edit_mode, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_isEditMode = edit_mode;

    QString userName = "";
    if(m_isEditMode)
    {
        QSqlQuery qry(m_database);
        QString textSQL =  QString("select id, projectname, description, deadline, fulfillmentdate, userid, chief from projectsview")
                + QString(" where id = %1").arg(currentProjectID);
        qry.exec(textSQL);
        while(qry.next())
        {
            m_nameText->setText(qry.value(1).toString());
            m_descriptionText->setText(qry.value(2).toString());
            m_planDateEdit->setDate(qry.value(3).toDate());
            m_factDateEdit->setDate(qry.value(4).toDate());
            userName = qry.value(5).toString();
        }
    }
    else
    {
        QDate today = QDate::currentDate();
        m_planDateEdit->setDate(QDate(today.year(), 12, 31));
        m_factDateEdit->setDate(QDate(9999, 12, 31));
    }

    // руководитель проекта добавляет или просматривает только свои проекты
    if(!(m_currentUser.role | 1))
    {
            m_chiefsList->setVisible(false);
            m_chiefLabel->setText(m_chiefLabel->text() + QString(" %1").arg(m_currentUser.name));
            return;
    }

    QSqlQuery qry(m_database);
    QString sqlText = "select id, name, role from users where role >0";
    qry.exec(sqlText);
    while (qry.next())
    {
        m_chiefsList->addItem(qry.value(1).toString());
    }

    if (m_isEditMode)
    {
       int idx = m_chiefsList->findText(userName);
       if(idx >= 0)
           m_chiefsList->setCurrentIndex(idx);
    }
    else
        m_chiefsList->setCurrentIndex(0);

}


void ProjectsCardDialog::on_ProjectsCardDialog_rejected()
{
    close();
}


void ProjectsCardDialog::on_ProjectsCardDialog_accepted()
{
    QString uname = m_chiefsList->currentText();
    long userid = getUserByName(uname);
    if(userid < 1) return;
    QSqlQuery qry(m_database);
    QString pname = m_nameText->text();
    QString desc = m_descriptionText->toPlainText();
    QString dp = m_planDateEdit->date().toString(Qt::ISODate);
    QString df = m_factDateEdit->date().toString(Qt::ISODate);
    QString qryText;
    if(!m_isEditMode) // новая запись
    {
        qryText = "insert into projects(name,description, deadline,fulfillmentdate,userid) ";
        qryText += QString("values('%1','%2','%3','%4',%5)")
                       .arg(pname).arg(desc).arg(dp).arg(df).arg(userid);
        if(!qry.exec(qryText))
        {
            QString errMsg = qry.lastError().text();
            QMessageBox::warning(this,"Ошибка",QString("Запись не добавлена: %1").arg(errMsg));
        }
    }
    else
    {
        qryText = QString("update projects set name = '%1', description = '%2', deadline = '%3',fulfillmentdate = '%4',userid= %5")
                .arg(pname).arg(desc).arg(dp).arg(df).arg(userid);
        qryText += QString(" where id = %1").arg(currentProjectID);
        if(!qry.exec(qryText))
        {
            QString errMsg = qry.lastError().text();
            QMessageBox::warning(this,"Ошибка",QString("Поля записи не изменены: %1").arg(errMsg));
        }

    }

}

