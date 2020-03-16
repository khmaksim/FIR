#include "databaseaccess.h"
#include <QSqlDatabase>
#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QSqlRecord>
#include <QDateTime>
#include <QApplication>
#include <QSettings>

DatabaseAccess::DatabaseAccess(QObject *parent) : QObject(parent)
{
    readSettings();

    if (QFile(fileNameDatabase).exists()) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
//        db.setHostName("localhost");
        db.setDatabaseName(QString("DRIVER={Microsoft Access Driver (*.mdb)};DSN='';DBQ=%1").arg(fileNameDatabase));
        if (db.open()) {
            QMessageLogger(0, 0, 0).info("Connect database");
            QMessageLogger(0, 0, 0).debug() << "file database" << fileNameDatabase;
        }
        else {
            QMessageLogger(0, 0, 0).warning("Failed to connect to the database");
            QMessageLogger(0, 0, 0).debug() << db.lastError().text();
            qDebug() << db.lastError().text();
        }
    }
}

DatabaseAccess* DatabaseAccess::getInstance()
{
    static DatabaseAccess instance;
    return &instance;
}

//DatabaseAccess* DatabaseAccess::getInstance()
//{
//    if(databaseAccess == 0)
//       databaseAccess = new DatabaseAccess;
//    return databaseAccess;
//}

void DatabaseAccess::readSettings()
{
    QSettings settings;

    settings.beginGroup("database");
    fileNameDatabase = settings.value("file").toString();
    settings.endGroup();
}

QVector<QVariantList> DatabaseAccess::getZones()
{
    QSqlQuery query(db);
    QVector<QVariantList> zones = QVector<QVariantList>();

    query.exec("SELECT cn.id_arpt, cn.name FROM ctr_name cn, ctr_point cp "
               "WHERE cn.id_arpt = cp.id GROUP BY cn.id_arpt, cn.name "
               "ORDER BY cn.name");
    while (query.next()) {
        QSqlRecord record = query.record();
        QVariantList list = QVariantList();
        for (int i = 0; i < record.count(); i++)
            list.append(record.value(i));

        zones.append(list);
    }
    return zones;
}

QVector<QVariantList> DatabaseAccess::getPoints(int id)
{
    QSqlQuery query(db);
    QVector<QVariantList> points = QVector<QVariantList>();
    qDebug() << id;

    query.prepare("SELECT lat, lon FROM ctr_point WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
        qDebug() << query.lastError().text() << query.lastQuery() << query.boundValues();

    while (query.next()) {
        QSqlRecord record = query.record();
        QVariantList list = QVariantList();
        for (int i = 0; i < record.count(); i++)
            list.append(record.value(i));

        points.append(list);
    }

    return points;
}

int DatabaseAccess::insertAirfield(const QString &icaoCodeAirfield, const QString &nameAirfield)
{
    QSqlQuery query(db);

    query.exec("BEGIN TRANSACTION");

    if (!nameAirfield.isEmpty()) {
        query.prepare("INSERT INTO airfield (name, code_icao) SELECT :name, :code_icao WHERE NOT EXISTS(SELECT 1 "
                      "FROM airfield WHERE name = :name AND code_icao = :code_icao)");
        query.bindValue(":name", nameAirfield);
        query.bindValue(":code_icao", icaoCodeAirfield);
        if (!query.exec()) {
            qDebug() << query.lastError().text() << query.lastQuery() << query.boundValues();
            return -1;
        }

        // get id airfield
        query.prepare("SELECT id FROM airfield WHERE name = ? AND code_icao = ?");
        query.addBindValue(nameAirfield);
        query.addBindValue(icaoCodeAirfield);
        if (!query.exec()) {
            qDebug() << query.lastError().text() << query.lastQuery() << query.boundValues();
            QSqlDatabase::database().rollback();
        }

        if (query.first())
            return query.value(0).toInt();
    }
    return -1;
}

void DatabaseAccess::insertObstracle(int idAirfield, QMap<QString, QString> obstracle)
{
    QSqlQuery query(db);

    for (int i = 0; i < obstracle.size(); i++) {
        query.prepare("INSERT OR REPLACE INTO obstracle (id, name, latitude, longitude, orthometric_height, night_marking, airfield, last_updated) "
                      "VALUES (:id, :name, :latitude, :longitude, :orthometric_height, :night_marking, :airfield, datetime('now','localtime'))");
        query.bindValue(":id", obstracle.value("id"));
        query.bindValue(":name", obstracle.value("name"));
        query.bindValue(":latitude", obstracle.value("latitude"));
        query.bindValue(":longitude", obstracle.value("longitude"));
        query.bindValue(":orthometric_height", obstracle.value("orthometric_height").toInt());
        query.bindValue(":night_marking", obstracle.value("night_marking"));
        query.bindValue(":airfield", idAirfield);
        if (!query.exec())
            qDebug() << query.lastError().text() << query.lastQuery() << query.boundValues();
    }
    query.exec("COMMIT");
//    if (!QSqlDatabase::database().commit()) {
//        QSqlDatabase::database().rollback();
//        qDebug() << "Rollback transaction";
//    }
}

bool DatabaseAccess::removeAirfield(int idAirfield)
{
    QSqlQuery query(db);

    query.prepare("DELETE FROM airfield WHERE id = ?");
    query.addBindValue(idAirfield);
    if (!query.exec()) {
        qDebug() << query.lastError().text() << query.lastQuery() << query.boundValues();
        return false;
    }
    return true;
}
