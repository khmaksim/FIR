#ifndef SORTSEARCHFILTEROBSTRACLEMODEL_H
#define SORTSEARCHFILTEROBSTRACLEMODEL_H

#include <QSortFilterProxyModel>

class SortSearchFilterObstracleModel : public QSortFilterProxyModel
{
        Q_OBJECT
    public:
        explicit SortSearchFilterObstracleModel(QObject *parent = nullptr);
        ~SortSearchFilterObstracleModel();

//        void setFilterRadius(QString, QString, int);

//    public slots:
//        void setFilterProperty(QString, QVariant);

    protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
//        bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

    private:
//        QStringList tags;
//        QStringList types;
//        QStringList areas;
//        int fromHeight;
//        int toHeight;
//        bool markingDay;
//        bool nightMarking;
//        double lat;
//        double lon;
//        int radius;
};

#endif // SORTSEARCHFILTEROBSTRACLEMODEL_H
