#include "taskreportmodel.h"

taskReportModel::taskReportModel(QObject *parent, bool workRecords) :QAbstractListModel(parent)
{
    m_pList = new QList<taskRecord>();
    m_wList = new QList<workRecord>();
    m_workRecords = workRecords;
}
taskReportModel::~taskReportModel()
{
    delete m_pList;
    delete m_wList;
}

bool taskReportModel::isWorkRecords()
{
    return m_workRecords;
}

// *** Перегруженные функции ***
QVariant taskReportModel::data(const QModelIndex &idx, int Role) const
{
    if(!idx.isValid())
        return QVariant();
    if(Role == Qt::DisplayRole || Role == Qt::EditRole)
    {
            if(!m_workRecords)
            {
                taskRecord p = m_pList->at(idx.row());
                switch(idx.column())
                {
                       case 0:
                        return p.m_name; break;
                       case 1:
                        return p.m_content; break;
                       case 2:
                        return p.m_planDate; break;
                       case 3:
                        return p.m_factDate; break;
                       case 4:
                        return p.m_spentTime.toString(); break;
                      default:
                        return  QVariant();
                }
            }
            else
            {
                workRecord w = m_wList->at(idx.row());
                switch(idx.column())
                {
                       case 0:
                        return w.m_name; break;
                       case 1:
                        return w.m_content; break;
                       case 2:
                        return w.m_Date; break;
                       case 3:
                        return w.m_spentTime.toString(); break;
                      default:
                       return  QVariant();
                }

            }
    }
    else
        return QVariant();
}

bool taskReportModel::setData(const QModelIndex &idx, const QVariant &val, int Role)
{
    if(idx.isValid() && Role == Qt::EditRole)
    {
        if(!m_workRecords)
        {
            taskRecord p = m_pList->at(idx.row());
            switch(idx.column())
            {
                 case 0:
                     p.m_name = val.value<QString>();                     
                     break;
                case 1:
                    p.m_content = val.value<QString>();
                    break;
                case 2:
                      p.m_planDate = val.value<QString>();
                      break;
                case 3:
                      p.m_factDate =  val.value<QString>();
                      break;
                case 4:
                      TimeSpan ts;
                      TimeSpan::Parse(val.value<QString>(), ts);
                      p.m_spentTime = ts;
                      break;
            }
            m_pList->replace(idx.row(), p);
        }
        else
        {
            workRecord w= m_wList->at(idx.row());
            switch(idx.column())
            {
                 case 0:
                     w.m_name = val.value<QString>();
                     break;
                case 1:
                      w.m_content = val.value<QString>();
                      break;
                case 2:
                      w.m_Date =  val.value<QString>();
                      break;
                case 3:
                      TimeSpan ts;
                      TimeSpan::Parse(val.value<QString>(), ts);
                      w.m_spentTime = ts;
                      break;
            }
            m_wList->replace(idx.row(), w);

        }
        emit dataChanged(idx, idx);
        return true;
    }
    return false;
}

int taskReportModel::rowCount(const QModelIndex & parent) const
{
    return !m_workRecords ? m_pList->size() : m_wList->size();

}
int taskReportModel::columnCount(const QModelIndex &idx) const
{
    return !m_workRecords ? 5 : 4;
}

Qt::ItemFlags taskReportModel::flags(const QModelIndex &idx) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(idx);
    return idx.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}

QVariant taskReportModel::headerData(int section, Qt::Orientation orientation, int Role ) const
{
    if(Role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
            case 0:
                return QString("Наименование\nзадачи");
            case 1:
                return  QString("Содержание\nработы");
            case 2:
                return !m_workRecords ? QString("Плановая\nдата") : QString("Дата");
            case 3:
                return !m_workRecords ? QString("Дата\nзавершения") : QString("Время");
            case 4:
                if(!m_workRecords)
                    return QString("Время");
                else
                    return QVariant();
            default:
                return QVariant();

        }
    }
    else if(orientation == Qt::Vertical)
    {
        return QString::number(section+1);
    }
    else
        return QVariant();
}

// *** Функции добавления и вставки ***
void taskReportModel::append(taskRecord p)
{
    int newRow =  m_pList->count()+1;

       beginInsertRows(QModelIndex(), newRow, newRow);
           m_pList->append(p);
       endInsertRows();
}
void taskReportModel::append(workRecord w)
{
    int newRow =  m_wList->count()+1;

       beginInsertRows(QModelIndex(), newRow, newRow);
           m_wList->append(w);
       endInsertRows();
}

void taskReportModel::deleteRow(int idx)
{
    beginRemoveRows(QModelIndex(), idx,idx);
    if(!m_workRecords)
        m_pList->removeAt(idx);
    else
        m_wList->removeAt(idx);
    endRemoveRows();
}

void taskReportModel::insertAt(int idx, taskRecord p)
{

    int newRow = idx;

    beginInsertRows(QModelIndex(), newRow, newRow);
        m_pList->insert(newRow,p);
    endInsertRows();
}

void taskReportModel::insertAt(int idx, workRecord w)
{

    int newRow = idx;

    beginInsertRows(QModelIndex(), newRow, newRow);
        m_wList->insert(newRow,w);
    endInsertRows();
}

void taskReportModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    if(!m_workRecords)
        m_pList->clear();
    else
        m_wList->clear();
    endRemoveRows();

}
