#include "participantsmodel.h"

ParticipantsModel::ParticipantsModel(QObject *parent) :QAbstractListModel(parent)
{
    m_pList = new QList<Participant>();
}
ParticipantsModel::~ParticipantsModel()
{
    delete m_pList;
}


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
                    return p.startTime;
                   case 3:
                    return p.endTime;
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
              p.startTime = val.value<QTime>();
              break;
            case 3:
               p.endTime = val.value<QTime>();
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

        }
    }
    if(orientation == Qt::Vertical)
    {
        return QString::number(section+1);
    }
}
void ParticipantsModel::append(Participant p)
{
    int newRow =  m_pList->count()+1;

       beginInsertRows(QModelIndex(), newRow, newRow);
           m_pList->append(p);
       endInsertRows();
}
