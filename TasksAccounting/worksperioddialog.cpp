#include "worksperioddialog.h"


worksPeriodDialog::worksPeriodDialog(QSqlDatabase database,  QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_database = database;
    QDate today = QDate::currentDate();
    //QDate d1 = QDate(today.year(), today.month(),1);
    //QDate d2 = QDate(today.year(), today.month(),today.daysInMonth());
    m_dateFromEdit->setDate(today);
    m_dateToEdit->setDate(today);
    m_model = new taskReportModel(nullptr, true);
    ShowResults();
    m_reportView->setModel(m_model);
    m_reportView->setColumnWidth(0, 300);
    m_reportView->setColumnWidth(1, 300);
    m_reportView->resizeRowsToContents();

}

worksPeriodDialog::~worksPeriodDialog()
{
    delete m_model;
}

void worksPeriodDialog::ShowResults()
{
    QString from = m_dateFromEdit->date().toString(Qt::ISODate);
    QString to = m_dateToEdit->date().toString(Qt::ISODate);
    m_model->clear();

    QString textQuery =  QString("select Name, Content, Date, TimeSpent from WorksView where Date between '%1' and '%2'")
            .arg(from)
            .arg(to);
    QSqlQuery qr(textQuery, m_database);
    qr.exec();
    while(qr.next())
    {
        workRecord wr;
        wr.m_name = qr.value(0).toString();
        wr.m_content = qr.value(1).toString();
        wr.m_Date  = qr.value(2).toString();
        QString stime = qr.value(3).toString();
        TimeSpan ts;
        TimeSpan::Parse(stime, ts);
        wr.m_spentTime = ts;
        m_model->append(wr);
    }

    int rows = m_model->rowCount();
    TimeSpan tsum;
    for(int i=0; i < rows; i++)
    {
        QModelIndex idx = m_model->index(i, 3);
        QString stime = m_model->data(idx, Qt::DisplayRole).toString();

         TimeSpan ts;
         if(TimeSpan::Parse(stime, ts))
            tsum += ts;
    }
    if(tsum == 0.0)
        m_sumTimeLabel->setText("Итого: 00:00");
    else
        m_sumTimeLabel->setText(QString("Итого: %1").arg(tsum.toString()));
        m_reportView->resizeRowsToContents();

}

void worksPeriodDialog::on_m_closeButton_clicked()
{
    accept();
}

void worksPeriodDialog::on_m_setTimeButton_clicked()
{
    ShowResults();
}
