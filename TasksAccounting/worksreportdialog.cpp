#include "worksreportdialog.h"
#include "timespan.h"

worksReportDialog::worksReportDialog(QSqlDatabase database,
                                     QString query,
                                     QWidget *parent) :  QDialog(parent)
{
    setupUi(this);
    m_database = database;
    m_sqlQuery = query;
    QDate today = QDate::currentDate();
    m_dateFromEdit->setDate(QDate(today.year(), today.month(),1));
    m_dateToEdit->setDate(QDate(today.year(), today.month(),today.daysInMonth()));
    m_model = new taskReportModel();
    ShowResults();
    m_reportView->setModel(m_model);
    m_reportView->setColumnWidth(0, 250);
    m_reportView->setColumnWidth(1, 350);
    m_reportView->resizeRowsToContents();
}
worksReportDialog::~worksReportDialog()
{
    delete m_model;
}

void worksReportDialog::on_m_setTimeButton_clicked()
{
    ShowResults();
}

void worksReportDialog::on_m_closeButton_clicked()
{
    accept();
}

void worksReportDialog::ShowResults()
{
    QString from = m_dateFromEdit->date().toString(Qt::ISODate);
    QString to = m_dateToEdit->date().toString(Qt::ISODate);
    m_model->clear();

    QString textQuery =  m_sqlQuery;
    textQuery.replace(QString(":d1"), from);
    textQuery.replace(QString(":d2"), to);
    QSqlQuery qr(textQuery, m_database);
    qr.exec();
    while(qr.next())
    {
        taskRecord rc;
        rc.m_name = qr.value(0).toString();
        rc.m_content = qr.value(1).toString();
        rc.m_planDate = qr.value(2).toString();
        rc.m_factDate  = qr.value(3).toString();
        QString stimes = qr.value(4).toString();

        QStringList ltimes = stimes.split(';');
        TimeSpan tts;
        for(int i=0 ; i < ltimes.size(); i++)
        {
            QString s1 = ltimes.at(i);
            TimeSpan ts;
            if(TimeSpan::Parse(s1, ts))
            {
                tts += ts;
            }
        }
        rc.m_spentTime = tts;
        m_model->append(rc);
    }

    int rows = m_model->rowCount();
    TimeSpan tsum;
    for(int i=0; i < rows; i++)
    {
        QModelIndex idx = m_model->index(i, 4);
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
