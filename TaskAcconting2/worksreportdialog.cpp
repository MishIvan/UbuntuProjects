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
    m_asc = false;
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

    QHeaderView *m_headerView= m_reportView->horizontalHeader();
    QObject::connect(m_headerView, SIGNAL(sectionClicked(int )),
                     this, SLOT(on_sectionColumnClicked(int)));
    m_flag = true;
    m_oldWidth = width();
    m_oldHeight = height();

}
worksReportDialog::~worksReportDialog()
{
    delete m_model;
}

void worksReportDialog::ShowResults(int col)
{
    QString from = m_dateFromEdit->date().toString(Qt::ISODate);
    QString to = m_dateToEdit->date().toString(Qt::ISODate);

    QString textQuery = QString("select * from public.get_works('%1', '%2')")
            .arg(from)
            .arg(to);
    switch(col)
    {
        case 1:
        textQuery +=  " order by content";
            break;
        case 2:
        textQuery +=  " order by deadline";
            break;
        case 3:
        textQuery +=  " order by fulfiimentdate";
            break;
        case 4:
        textQuery +=  " order by timespent";
            break;
        case 0:
        default:
        textQuery +=  " order by name";
            break;
    }

    textQuery += m_asc ? " asc" : " desc";
    m_model->setQuery(textQuery, m_database);
    QSqlQuery qr(m_database);

    textQuery = QString("select sum(timespent) from public.get_works('%1', '%2')")
            .arg(from)
            .arg(to);
    qr.exec(textQuery);
    m_asc = !m_asc;
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

void worksReportDialog::resizeEvent(QResizeEvent *evt)
{
    int h = evt->size().height();
    int w = evt->size().width();
    int deltax = w - m_oldWidth;
    int deltay = h - m_oldHeight;

    m_reportView->setGeometry(m_reportView->x(), m_reportView->y(),
                                  m_reportView->width() + deltax,
                                  m_reportView->height() + deltay);
    m_reportView->updateGeometry();

    m_sumTimeLabel->setGeometry(m_sumTimeLabel->x() + deltax, m_sumTimeLabel->y() + deltay,
                                m_sumTimeLabel->width(), m_sumTimeLabel->height()
                                );
    m_sumTimeLabel->updateGeometry();
    m_oldWidth = w;
    m_oldHeight = h;
}
void worksReportDialog::on_sectionColumnClicked(int col)
{
    ShowResults(col);
}
