#include "worksdialog.h"

worksDialog::worksDialog(QSqlTableModel *model,QModelIndex idx,  QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    m_tasksModel = model;
    m_taskIndex = idx;
    m_worksModel = new QSqlTableModel(nullptr, m_tasksModel->database());

    m_worksModel->setTable("Works");

    int row = m_taskIndex.row();
    m_taskID = m_tasksModel->data(m_tasksModel->index(row,0)).toULongLong();
    m_taskName->setText(m_tasksModel->data(m_tasksModel->index(row,1)).toString() );
    m_taskDescription->setPlainText(m_tasksModel->data(m_tasksModel->index(row,2)).toString() );
    QString filter = QString("TaskID=%1").arg(m_taskID);
    m_worksModel->setFilter(filter);
    m_worksModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_worksModel->select();

    m_worksModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m_worksModel->setHeaderData(1, Qt::Horizontal, QObject::tr("ID задачи"));
    m_worksModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Описание"));
    m_worksModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Дата"));
    m_worksModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Затраченное\nвремя"));
    worksTableView->setModel(m_worksModel);

    worksTableView->hideColumn(0);
    worksTableView->hideColumn(1);
    worksTableView->setColumnWidth(2, 350);

}

worksDialog::~worksDialog()
{
    delete m_worksModel;
}

void worksDialog::on_closeButton_clicked()
{
    setResult(QDialog::Accepted);
    close();
}

void worksDialog::on_addTaskButton_clicked()
{
    workRecDialog wrd(this);
    if(wrd.exec() == QDialog::Accepted)
    {
         int row = m_taskIndex.row();
         row = m_worksModel->rowCount();
         m_worksModel->insertRows(row,1);
         m_worksModel->setData(m_worksModel->index(row,1), QVariant((qlonglong)m_taskID));
         m_worksModel->setData(m_worksModel->index(row,2), wrd.m_contents);
         m_worksModel->setData(m_worksModel->index(row,3), wrd.m_date.toString(Qt::ISODate));
         m_worksModel->setData(m_worksModel->index(row,4), wrd.m_time);
         m_worksModel->submitAll();
         m_worksModel->setFilter(QString("TaskID=%1").arg(m_taskID));
         m_worksModel->select();
    }

}

void worksDialog::on_delTaskButton_clicked()
{
    QItemSelectionModel *selmodel =  worksTableView->selectionModel();
    QModelIndex idx = selmodel->currentIndex();

    m_worksModel->removeRow(idx.row());
    m_worksModel->submitAll();
    m_worksModel->select();

}
// при закрытии диалога рассчитать общее время, затраченное на выполнение задачи
void worksDialog::closeEvent(QCloseEvent *evt)
{    
    int rows = m_worksModel->rowCount();
    if(rows < 1) return;
    int row = m_taskIndex.row();
    QString tm = m_tasksModel->data(m_tasksModel->index(row,6)).toString();
    TimeSpan tss;
    for(int i = 0; i < rows; i++)
    {
        QString stime = m_worksModel->data(m_worksModel->index(i, 4)).toString();
        TimeSpan ts;
        if( TimeSpan::Parse(stime, ts))
            tss += ts;
    }
    if(tss == 0.0 || tss.toString() == tm) return;
    m_tasksModel->setData(m_tasksModel->index(row,6), tss.toString());
    m_tasksModel->submitAll();
    m_tasksModel->select();
}

void worksDialog::on_editTaskButton_clicked()
{

    workRecDialog wrd(this);
    QItemSelectionModel *selmodel =  worksTableView->selectionModel();
    QModelIndex idx = selmodel->currentIndex();
    int row = idx.row();

    wrd.m_contents = m_worksModel->data(m_worksModel->index(row,2)).toString();
    wrd.m_date = QDate::fromString(m_worksModel->data(m_worksModel->index(row,3)).toString(),
                                   Qt::ISODate);
    wrd.m_time = m_worksModel->data(m_worksModel->index(row,4)).toString();
    wrd.Initialize();
    if(wrd.exec() == QDialog::Accepted)
    {
         m_worksModel->setData(m_worksModel->index(row,2), wrd.m_contents);
         m_worksModel->setData(m_worksModel->index(row,3), wrd.m_date.toString(Qt::ISODate));
         m_worksModel->setData(m_worksModel->index(row,4), wrd.m_time);
         m_worksModel->submitAll();
         m_worksModel->setFilter(QString("TaskID=%1").arg(m_taskID));
         m_worksModel->select();
    }
}
