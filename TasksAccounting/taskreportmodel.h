#ifndef PARTICIPANTSMODEL_H
#define PARTICIPANTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
//#include "timespan.h"
#include "sorting.hpp"
#include <QDate>

struct taskRecord
{
    QString m_name;
    QString m_content;
    QDate m_planDate;
    QDate m_factDate;
    QString m_spentTime;
};
struct workRecord
{
    QString m_name;
    QString m_content;
    QDate m_Date;
    QString m_spentTime;
};

Q_DECLARE_METATYPE(taskRecord);
Q_DECLARE_METATYPE(workRecord);
class taskReportModel : public QAbstractListModel
{
    Q_OBJECT
    QList<taskRecord> *m_pList;
    QList <workRecord> *m_wList;
    bool m_workRecords;
public:
    taskReportModel(QObject *parent = nullptr, bool workRecords = false);
    ~taskReportModel();

    QVariant data(const QModelIndex &idx, int Role) const;
    bool setData(const QModelIndex &idx, const QVariant &val, int Role);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &idx) const;
    Qt::ItemFlags flags(const QModelIndex &idx) const;
    QVariant headerData(int section, Qt::Orientation orientation, int Role = Qt::DisplayRole) const;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    void append(taskRecord p);
    void append(workRecord w);
    void deleteRow(int idx);
    void insertAt(int idx, taskRecord p);
    void insertAt(int idx, workRecord w);
    void clear();



    bool isWorkRecords();

};

#endif // PARTICIPANTSMODEL_H
