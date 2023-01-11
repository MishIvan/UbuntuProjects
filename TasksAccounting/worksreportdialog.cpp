#include "worksreportdialog.h"
#include "timespan.h"
void GetAccontingPeriod(QDate &, QDate &);

worksReportDialog::worksReportDialog(QSqlDatabase database,
                                     QString query,
                                     QWidget *parent) :  QDialog(parent)
{
    setupUi(this);
    m_flag = false;
    m_database = database;
    m_sqlQuery = query;
    //QDate today = QDate::currentDate();
    //m_dateFromEdit->setDate(QDate(today.year(), today.month(),1));
    //m_dateToEdit->setDate(QDate(today.year(), today.month(),today.daysInMonth()));
    QDate dateBegin, dateEnd;
    GetAccontingPeriod(dateBegin, dateEnd);
    m_dateFromEdit->setDate(dateBegin);
    m_dateToEdit->setDate(dateEnd);

    m_model = new taskReportModel();
    ShowResults();
    m_reportView->setModel(m_model);
    m_reportView->setColumnWidth(0, 250);
    m_reportView->setColumnWidth(1, 350);
    m_reportView->resizeRowsToContents();
    m_flag = true;
}
worksReportDialog::~worksReportDialog()
{
    delete m_model;
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
    QSqlQuery qr(m_database);
    qr.exec(textQuery);
    while(qr.next())
    {
        taskRecord rc;
        rc.m_name = qr.value(0).toString();
        rc.m_content = qr.value(1).toString();
        rc.m_planDate = qr.value(2).toString();
        rc.m_factDate  = qr.value(3).toString();
        QString stimes = qr.value(4).toString();

        TimeSpan tts;

        /*QStringList ltimes = stimes.split(';');
        for(int i=0 ; i < ltimes.size(); i++)
        {
            QString s1 = ltimes.at(i);
            TimeSpan ts;
            if(TimeSpan::Parse(s1, ts))
            {
                tts += ts;
            }
        } */

        TimeSpan::Parse(stimes, tts);
        rc.m_spentTime = tts;
        m_model->append(rc);
    }

    /*int rows = m_model->rowCount();
    TimeSpan tsum;
    for(int i=0; i < rows; i++)
    {
        QModelIndex idx = m_model->index(i, 4);
        QString stime = m_model->data(idx, Qt::DisplayRole).toString();

         TimeSpan ts;
         if(TimeSpan::Parse(stime, ts))
            tsum += ts;
    }*/

    qr.exec(QString("select sum(timespent) ts from ( %1 ) t;").arg(textQuery));
    QString stime;
    while(qr.next())
    {
        stime = qr.value(0).toString();
    }

    TimeSpan ts;
    TimeSpan::Parse(stime, ts);

    if(ts == 0.0)
        m_sumTimeLabel->setText("Итого: 00:00");
    else
        m_sumTimeLabel->setText(QString("Итого: %1").arg(ts.toString()));
    m_reportView->resizeRowsToContents();
}

void worksReportDialog::on_m_dateFromEdit_userDateChanged(const QDate &date)
{
    if(m_flag) ShowResults();
}

void worksReportDialog::on_m_dateToEdit_userDateChanged(const QDate &date)
{
    if(m_flag) ShowResults();
}
