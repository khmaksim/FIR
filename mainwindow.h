#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class Database;
//class ObstraclesHandler;
class QStandardItemModel;
class QToolButton;
class MapView;
class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:
        void showObstracles();

    private:
        Ui::MainWindow *ui;
        QToolBar *toolBar;
        QToolButton *exportButton;
        QToolButton *displayOnMapButton;
        QToolButton *settingsButton;
        MapView *mapView;

        QStandardItemModel *model;
//        ObstraclesHandler *obstraclesHandler;
        Database *db;

        void updateZoneTable();
        void readSettings();
        void writeSettings();
        /*void showZones();*///QVariant coordinate, QVariant radius);

    private slots:
        void enabledToolButton();
        void showZones();
        void showSettings();
        void exportToFile();
};

#endif // MAINWINDOW_H
