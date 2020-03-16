#ifndef DATABASEACCESS_H
#define DATABASEACCESS_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseAccess : public QObject
{
        Q_OBJECT
    public:
        static DatabaseAccess* getInstance();

        QVector<QVariantList> getZones();
        QVector<QVariantList> getPoints(int id);
        int insertAirfield(const QString&, const QString&);
        void insertObstracle(int idAirfield, QMap<QString, QString> obstracle);
        bool removeAirfield(int idAirfield);

    private:
        DatabaseAccess(QObject *parent = nullptr);
        DatabaseAccess(const DatabaseAccess&);
        DatabaseAccess& operator =(const DatabaseAccess);
//        static DatabaseAccess *databaseAccess;

        QSqlDatabase db;
        QString fileNameDatabase;

        void readSettings();

    signals:
        void updatedTags();
};

#endif // DATABASEACCESS_H
