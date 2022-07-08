#include "taskreportmodel.h"

taskReportModel::taskReportModel(QObject *parent) :QAbstractListModel(parent)
{
    m_pList = new QList<taskRecord>();
}
taskReportModel::~taskReportModel()
{
    delete m_pList;
}

// *** Перегруженные функции ***
QVariant taskReportModel::data(const QModelIndex &idx, int Role) const
{
    if(!idx.isValid())
        return QVariant();
    if(Role == Qt::DisplayRole || Role == Qt::EditRole)
    {
            taskRecord p = m_pList->at(idx.row());
            switch(idx.column())
            {
                   case 0:
                    return p.m_name;
                   case 1:
                    return p.m_planDate;
                   case 2:
                    return p.m_factDate;
                   case 3:
                    return p.m_spentTime.toString();
            }
    }
    else
        return QVariant();
}

bool taskReportModel::setData(const QModelIndex &idx, const QVariant &val, int Role)
{
    if(idx.isValid() && Role == Qt::EditRole)
    {
        taskRecord p = m_pList->at(idx.row());
        switch(idx.column())
        {
             case 0:
                 p.m_name = val.value<QString>();
                 break;
            case 1:
                  p.m_planDate = val.value<QString>();
                  break;
            case 2:
                  p.m_factDate =  val.value<QString>();
                  break;
            case 3:
                  TimeSpan ts;
                  TimeSpan::Parse(val.value<QString>(), ts);
                  p.m_spentTime = ts;
                  break;


        }
        m_pList->replace(idx.row(), p);
        emit dataChanged(idx, idx);
        return true;
    }
    return false;
}

int taskReportModel::rowCount(const QModelIndex & parent) const
{
    return m_pList->size();

}
int taskReportModel::columnCount(const QModelIndex &idx) const
{
    return 4;
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
                return QString("Срок");
            case 2:
                return QString("Дата\nзавершения");
            case 3:
                return QString("Время");
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
void taskReportModel::deleteRow(int idx)
{
    beginRemoveRows(QModelIndex(), idx,idx);
        m_pList->removeAt(idx);
    endRemoveRows();
}

void taskReportModel::insertAt(int idx, taskRecord p)
{

    int newRow = idx;

    beginInsertRows(QModelIndex(), newRow, newRow);
        m_pList->insert(newRow,p);
    endInsertRows();
}

void taskReportModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        m_pList->clear();
    endRemoveRows();

}
