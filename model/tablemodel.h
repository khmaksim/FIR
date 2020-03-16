#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QStandardItemModel>

class TableModel : public QStandardItemModel
{
        Q_OBJECT
    public:
        explicit TableModel(QObject *parent = nullptr);
        ~TableModel();

        bool setData(const QModelIndex &index, const QVariant &value, int role);
        QVariant data(const QModelIndex &index, int role) const;
};

#endif // TABLEMODEL_H
