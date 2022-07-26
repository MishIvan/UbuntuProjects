#include "taskfilterdialog.h"

taskFilterDialog::taskFilterDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}
int taskFilterDialog::filterFlag()
{
    return m_filterFlag;
}
void taskFilterDialog::setFilterFlag(int flag)
{
    m_filterFlag = flag;
    switch(m_filterFlag)
    {
        case TaskFilter::ALL:
        m_AllRadioButton->setChecked(true);
        break;
        case TaskFilter::DONE:
        m_DoneRadioButton->setChecked(true);
        break;
        case TaskFilter::INFINITE:
        m_InfiniteRadioButton->setChecked(true);
        break;
        case TaskFilter::EXPIRED:
        m_ExpiredRadioButton->setChecked(true);
        break;
        default:
        break;
    };

}
void taskFilterDialog::on_buttonBox_accepted()
{
    if(m_AllRadioButton->isChecked())
        m_filterFlag = TaskFilter::ALL;
    if(m_DoneRadioButton->isChecked())
        m_filterFlag = TaskFilter::DONE;
    if(m_ExpiredRadioButton->isChecked())
        m_filterFlag = TaskFilter::EXPIRED;
    if(m_InfiniteRadioButton->isChecked())
        m_filterFlag = TaskFilter::INFINITE;
}

void taskFilterDialog::on_buttonBox_rejected()
{
    close();
}
