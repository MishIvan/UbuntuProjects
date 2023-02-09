#include "setperioddialog.h"
#include <QSettings>
#include <QDate>

void GetAccontingPeriod(QDate &, QDate &);
void SetAccountingPeriod(const QDate &, const QDate &);

setPeriodDialog::setPeriodDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QDate dateBegin, dateEnd;
    GetAccontingPeriod(dateBegin, dateEnd);
    m_dateBegin->setDate(dateBegin);
    m_dateEnd->setDate(dateEnd);

}

void setPeriodDialog::on_buttonBox_accepted()
{
    QDate dateBegin, dateEnd;
    dateBegin = m_dateBegin->date();
    dateEnd = m_dateEnd->date();
    SetAccountingPeriod(dateBegin, dateEnd);
}
