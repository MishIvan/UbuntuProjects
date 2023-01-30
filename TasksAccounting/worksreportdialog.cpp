#include "worksreportdialog.h"

void GetAccontingPeriod(QDate &, QDate &);

worksReportDialog::worksReportDialog(QSqlDatabase database,
                                     QWidget *parent) :  QDialog(parent)
{
    setupUi(this);
    m_flag = false;
    m_database = database;

    QDate dateBegin, dateEnd;
    GetAccontingPeriod(dateBegin, dateEnd);
    m_dateFromEdit->setDate(dateBegin);
    m_dateToEdit->setDate(dateEnd);

    //m_model = new taskReportModel();
    m_model = new QSqlQueryModel;
    ShowResults();

    m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Задача"));
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Список\nработ"));
    m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Срок\nзавершения"));
    m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Завершена"));
    m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Время"));

    m_reportView->setModel(m_model);
    m_reportView->setColumnWidth(0, 250);
    m_reportView->setColumnWidth(1, 400);
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
    //m_model->clear();

    QString textQuery = QString("select * from public.get_works('%1', '%2')")
            .arg(from)
            .arg(to);
    m_model->setQuery(textQuery, m_database);
    QSqlQuery qr(m_database);
   /* qr.exec(textQuery);
    while(qr.next())
    {
        taskRecord rc;
        rc.m_name = qr.value(0).toString();
        rc.m_content = qr.value(1).toString();
        rc.m_planDate = qr.value(2).toDate();
        rc.m_factDate  = qr.value(3).toDate();
        rc.m_spentTime = qr.value(4).toString();

        m_model->append(rc);
    } */

    textQuery = QString("select sum(timespent) from public.get_works('%1', '%2')")
            .arg(from)
            .arg(to);
    qr.exec(textQuery);
    QString stime("");
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

void worksReportDialog::on_m_dateFromEdit_userDateChanged(const QDate &date)
{
    if(m_flag) ShowResults();
}

void worksReportDialog::on_m_dateToEdit_userDateChanged(const QDate &date)
{
    if(m_flag) ShowResults();
}
