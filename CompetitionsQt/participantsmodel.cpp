#include "participantsmodel.h"

ParticipantsModel::ParticipantsModel(QObject *parent) :QAbstractListModel(parent)
{
    m_pList = new QList<Participant>();
}
ParticipantsModel::~ParticipantsModel()
{
    delete m_pList;
}

// *** Перегруженные функции ***
QVariant ParticipantsModel::data(const QModelIndex &idx, int Role) const
{
    if(!idx.isValid())
        return QVariant();
    if(Role == Qt::DisplayRole || Role == Qt::EditRole)
    {
            Participant p = m_pList->at(idx.row());
            switch(idx.column())
            {
                   case 0:
                    return p.name;
                   case 1:
                    return p.age;
                   case 2:
                    return p.startTime.toString();
                   case 3:
                    return p.endTime.toString();
            }
    }
    else
        return QVariant();
}

bool ParticipantsModel::setData(const QModelIndex &idx, const QVariant &val, int Role)
{
    if(idx.isValid() && Role == Qt::EditRole)
    {
        Participant p = m_pList->at(idx.row());
        switch(idx.column())
        {
             case 0:
                 p.name = val.value<QString>();
                 break;
            case 1:
                  p.age = val.value<int>();
                  break;
            case 2:
              p.startTime = QTime::fromString(val.value<QString>());
              break;
            case 3:
               p.endTime = QTime::fromString(val.value<QString>());
               break;


        }
        m_pList->replace(idx.row(), p);
        emit dataChanged(idx, idx);
        return true;
    }
    return false;
}

int ParticipantsModel::rowCount(const QModelIndex & parent) const
{
    return m_pList->size();

}
int ParticipantsModel::columnCount(const QModelIndex &idx) const
{
    return 4;
}

Qt::ItemFlags ParticipantsModel::flags(const QModelIndex &idx) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags(idx);
    return idx.isValid() ? (flags | Qt::ItemIsEditable) : flags;
}

QVariant ParticipantsModel::headerData(int section, Qt::Orientation orientation, int Role ) const
{
    if(Role != Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
            case 0:
                return QString("ФИО");
            case 1:
                return QString("Возраст");
            case 2:
                return QString("Старт");
            case 3:
                return QString("Финиш");
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
void ParticipantsModel::append(Participant p)
{
    int newRow =  m_pList->count()+1;

       beginInsertRows(QModelIndex(), newRow, newRow);
           m_pList->append(p);
       endInsertRows();
}
void ParticipantsModel::deleteRow(int idx)
{
    beginRemoveRows(QModelIndex(), idx,idx);
        m_pList->removeAt(idx);
    endRemoveRows();
}

void ParticipantsModel::insertAt(int idx, Participant p)
{

    int newRow = idx;

    beginInsertRows(QModelIndex(), newRow, newRow);
        m_pList->insert(newRow,p);
    endInsertRows();
}

void ParticipantsModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        m_pList->clear();
    endRemoveRows();

}
