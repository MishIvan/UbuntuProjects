#include "worksperioddialog.h"

void GetAccontingPeriod(QDate &, QDate &);
worksPeriodDialog::worksPeriodDialog(QSqlDatabase database,  QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_flag = false;
    m_database = database;
    //QDate today = QDate::currentDate();
    //QDate d1 = QDate(today.year(), today.month(),1);
    //QDate d2 = QDate(today.year(), today.month(),today.daysInMonth());
    QDate dateBegin, dateEnd;
    GetAccontingPeriod(dateBegin, dateEnd);
    m_dateFromEdit->setDate(dateBegin);
    m_dateToEdit->setDate(dateEnd);
    //m_model = new taskReportModel(nullptr, true);
   m_model = new QSqlQueryModel;

   ShowResults();

   m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Наименование"));
   m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Содержание"));
   m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Дата"));
   m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Время"));

    m_reportView->setModel(m_model);
    m_reportView->setColumnWidth(0, 300);
    m_reportView->setColumnWidth(1, 300);

    m_reportView->resizeRowsToContents();
    m_reportView->show();
    m_flag = true;
    m_periodCheckBox->setCheckState(Qt::Checked);
}

worksPeriodDialog::~worksPeriodDialog()
{
    delete m_model;
}

void worksPeriodDialog::ShowResults()
{
    QString from = m_dateFromEdit->date().toString(Qt::ISODate);
    QString to = m_dateToEdit->date().toString(Qt::ISODate);
    //m_model->clear();

    QString textQuery =  QString("select name, content, date, timespent from worksview where date between '%1' and '%2' order by date")
            .arg(from)
            .arg(to);
    m_model->setQuery(textQuery, m_database);

    QSqlQuery qr(m_database);
    /*qr.exec(textQuery);
    QString stime;
    while(qr.next())
    {
        workRecord wr;

        wr.m_name = qr.value(0).toString();
        wr.m_content = qr.value(1).toString();
        wr.m_Date  = qr.value(2).toDate();
        wr.m_spentTime = qr.value(3).toString();

        m_model->append(wr);
    } */

    textQuery =  QString("select sum(timespent) from worksview where date between '%1' and '%2'")
            .arg(from)
            .arg(to);
    qr.exec(textQuery);
    QString stime = "";
    while(qr.next())
    {
        stime = qr.value(0).toString();
    }
    if(stime.isEmpty())
        m_sumTimeLabel->setText("Итого: 00:00:00");
    else
        m_sumTimeLabel->setText(QString("Итого: %1").arg(stime));
    m_reportView->resizeRowsToContents();

}

void worksPeriodDialog::on_m_closeButton_clicked()
{
    accept();
}

void worksPeriodDialog::on_m_todayCheckBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        QDate today = QDate::currentDate();
        m_dateFromEdit->setDate(today);
        m_dateToEdit->setDate(today);
        if(m_periodCheckBox->isChecked())
            m_periodCheckBox->setChecked(false);
        m_todayCheckBox->setCheckState(Qt::Checked);
        ShowResults();
    }
}

void worksPeriodDialog::on_m_periodCheckBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        QDate dateBegin, dateEnd;
        GetAccontingPeriod(dateBegin, dateEnd);
        m_dateFromEdit->setDate(dateBegin);
        m_dateToEdit->setDate(dateEnd);
        if(m_todayCheckBox->isChecked())
            m_todayCheckBox->setChecked(false);
        m_periodCheckBox->setCheckState(Qt::Checked);
        ShowResults();
    }

}

void worksPeriodDialog::on_m_dateFromEdit_userDateChanged(const QDate &date)
{
    if(m_periodCheckBox->isChecked())
        m_periodCheckBox->setChecked(Qt::Unchecked);
    if(m_todayCheckBox->isChecked())
        m_todayCheckBox->setChecked(Qt::Unchecked);
    if (m_flag) ShowResults();
}

void worksPeriodDialog::on_m_dateToEdit_userDateChanged(const QDate &date)
{
    on_m_dateFromEdit_userDateChanged(date);
}
