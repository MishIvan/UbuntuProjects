#ifndef PARTICIPANTSMODEL_H
#define PARTICIPANTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <Helper.h>

Q_DECLARE_METATYPE(Participant);
class ParticipantsModel : public QAbstractListModel
{
    Q_OBJECT
    QList<Participant> *m_pList;
public:
    ParticipantsModel(QObject *parent = nullptr);
    ~ParticipantsModel();

    QVariant data(const QModelIndex &idx, int Role) const;
    bool setData(const QModelIndex &idx, const QVariant &val, int Role);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &idx) const;
    Qt::ItemFlags flags(const QModelIndex &idx) const;
    QVariant headerData(int section, Qt::Orientation orientation, int Role = Qt::DisplayRole) const;

    void append(Participant p);
    void deleteRow(int idx);
    void insertAt(int idx, Participant p);
    void clear();

};

#endif // PARTICIPANTSMODEL_H
